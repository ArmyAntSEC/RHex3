/* Homing Encoder Library for Arduino Due
 * Copyright 2020 Daniel Armyr
 * 
 * The core algorithm, some macros and lines of code were taken from:
 * Encoder Library, for measuring quadrature encoded signals
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 * Copyright (c) 2011,2013 PJRC.COM, LLC - Paul Stoffregen <paul@pjrc.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _HOMINGENCODER_H_
#define _HOMINGENCODER_H_

#include <FixedPointsCommon.h>
#include <LevelLogger.h>
#include "RotationPositionWithLaps.h"
#include "HomingEncoderHardwareInterface.h"
#define MAX_ENCODERS_SUPPORTED 6


struct HomingEncoder
{
  HomingEncoderHardwareInterface nativeInterface;

  static const SQ15x16 clicksPerRevolution;
  const int speedTimeConstant = 10;

  int encoderPin1;
  int encoderPin2;
  int homingPin;

  //TODO: Convert this to an object with volatile members that can atomically generate a copy of itself.
  volatile long int raw_position;
  volatile long int laps;
  SQ1x14 position_remainder;

  volatile bool is_homed;
  //TODO: Use the above isr-safe object to also handle these things to that we can handle pos at last home in a more robust manner.
  volatile long int pos_at_last_home;
  volatile long int laps_at_last_home;

  //Also use the above object here as well.
  long int last_position;
  unsigned long int last_position_timestamp_micros;
  SQ15x16 speed_cps;
  SQ15x16 speed_cps_filtered;

  void config( unsigned int _encoderPin1,
              unsigned int _encoderPin2, unsigned int _breakerPin,
              int _offset )
  {      
    encoderPin1 = _encoderPin1;
    encoderPin2 = _encoderPin2;
    homingPin = _breakerPin;
    
    nativeInterface.configurePins( encoderPin1, encoderPin2, homingPin );

    raw_position = 0;
    laps = 0;
    position_remainder = 0;

    is_homed = false;
    pos_at_last_home = 0;
    last_position = 0;
    last_position_timestamp_micros = 0;
    speed_cps = 0;    
  }

  long int getRawPos()
  {
    long int r;
    nativeInterface.disableInterrupts();
    r = raw_position;
    nativeInterface.enableInterrupts();
    return r;
  }

  RotationPositionWithLaps getPosition()
  {
    return RotationPositionWithLaps(
      this->getRawPos(), this->laps, this->position_remainder );
  }

  void handleOverflow()
  {
    int clicksPerRevInt = clicksPerRevolution.getInteger();
    SQ15x16 precise_position = 0;

    nativeInterface.disableInterrupts();
    if (raw_position > clicksPerRevInt)
    {
      precise_position = SQ15x16((long int)raw_position) + 
        SQ15x16(position_remainder) - clicksPerRevolution;
      raw_position = floorFixed(precise_position).getInteger();
      position_remainder = SQ1x14(precise_position - floorFixed(precise_position));
      laps++;
    }
    nativeInterface.enableInterrupts();
  }

  //Should be called once every 10ms to compute the speed.
  virtual void run()
  {
    
    //Calculate the max range for values:
    //Max rpm: 3
    //Max clicks per second: 3*3500 = 10000
    //Max clicks per 0.01 s = 100

    //Min clicks per second 4700
    //Min clicks per 0.01s = 47

    unsigned long int nowU = nativeInterface.getMicrosecondsSinceBoot();

    int thisPos = this->getRawPos();    
    int lastPos = last_position;
    unsigned long int lastTimeU = last_position_timestamp_micros;    

    SQ15x16 posDelta = positionPositiveDifference(thisPos, lastPos);
    last_position = thisPos;

    SQ15x16 timeDelta = nowU - lastTimeU;
    last_position_timestamp_micros = nowU;

    SQ15x16 speedCPS;
    if (timeDelta == 0)
    {
      speedCPS = 0;
    }
    else
    {
      //First pre-scale by 100, then divide, then scale by 10k for a total of 1e6.
      speedCPS = 10000 * ((100 * posDelta) / timeDelta);
    }

    last_position = thisPos;
    last_position_timestamp_micros = nowU;
    speed_cps = speedCPS;
    speed_cps_filtered =
        ((speed_cps_filtered +
          speed_cps / (this->speedTimeConstant - 1)) /
         this->speedTimeConstant) *
        (this->speedTimeConstant - 1);

    //Make sure that we handle any overflows.    
    handleOverflow();    
  }
  
  static long int positionPositiveDifference(long int pos1, long int pos2)
  {
    if (pos1 >= pos2)
      return pos1 - pos2;
    else
      return clicksPerRevolution.getInteger() + pos1 - pos2;
  }  

   long int getSpeedCPS()
  {
    return speed_cps.getInteger();
  }

  long int getSpeedCPSFiltered()
  {
    return speed_cps_filtered.getInteger();
  }
  
  void unHome()
  {
    nativeInterface.disableInterrupts();
    is_homed = false;    
    nativeInterface.enableInterrupts();
  }

  void forceHomed()
  {
    nativeInterface.disableInterrupts();
    is_homed = true;
    pos_at_last_home = raw_position;
    laps_at_last_home = laps;
    raw_position = 0;    
    laps = 0;
    nativeInterface.enableInterrupts();  
  }

  bool isHomed()
  {
    nativeInterface.disableInterrupts();
    bool _is_homed = is_homed;
    nativeInterface.enableInterrupts();
    return _is_homed;
  }

  int getPosAtLastHome()
  {
    nativeInterface.disableInterrupts();
    int rValue = pos_at_last_home;
    nativeInterface.enableInterrupts();
    return rValue;
  }

  int getLapsAtLastHome()
  {
    nativeInterface.disableInterrupts();
    int rValue = laps_at_last_home;
    nativeInterface.enableInterrupts();
    return rValue;
  }

  unsigned int getHomingPinValue()
  {
    return nativeInterface.getDigitalValueFromPin( homingPin );
  }

};

class HomingEncoderFactory
{
private:
  HomingEncoderHardwareInterface nativeInterface;

public:    
  static HomingEncoder stateList[MAX_ENCODERS_SUPPORTED];  

  template <int N> HomingEncoder* config(
    unsigned int encoderPin1, unsigned int encoderPin2, 
    unsigned int homingPin, int offset )
  {
    static_assert(N < MAX_ENCODERS_SUPPORTED);    
    HomingEncoder* state = &stateList[N];
         
    state->config( encoderPin1, encoderPin2, homingPin, offset );
    
    //Only trigger homing on rising or we home twice per rotation
    nativeInterface.attachAnInterrupt( homingPin, isr_homing<N>, HomingEncoderHardwareInterface::TriggerModes::_FALLING );
    nativeInterface.attachAnInterrupt( encoderPin1, isr_encoder<N>, HomingEncoderHardwareInterface::TriggerModes::_CHANGE );
        
    return state;
  }
  
  template <int N> static void isr_encoder(void)
  {    
    HomingEncoder *state = &stateList[N];
    state->raw_position++;   
  }

  template <int N> static void isr_homing(void)
  {    
    HomingEncoder *state = &stateList[N];        

    if ( !(state->is_homed) && state->raw_position > 200 ) {  //Do some debouncing.
      state->is_homed = true;
      state->pos_at_last_home = state->raw_position;
      state->laps_at_last_home = state->laps;
      state->raw_position = 0;
      state->laps = 0;
    }
  }  
};

#endif
