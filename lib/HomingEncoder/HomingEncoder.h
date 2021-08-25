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

#include <Arduino.h>
#include <FixedPointsCommon.h>
#include <LevelLogger.h>
#include "RotationPositionWithLaps.h"

#define PIN_TO_BASEREG(pin) (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin) (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask) (((*(base)) & (mask)) ? 1 : 0)

#define MAX_ENCODERS_SUPPORTED 6

struct HomingEncoderState
{
  static const SQ15x16 clicksPerRevolution;
  const int speedTimeConstant = 10;

  int encoderPin1;
  int encoderPin2;
  int breakerPin;

  volatile long int raw_position;
  long int laps;
  SQ1x14 position_remainder;

  volatile bool is_homed;
  volatile long int pos_at_last_home;

  long int last_position;
  unsigned long int last_position_timestamp_micros;
  SQ15x16 speed_cps;
  SQ15x16 speed_cps_filtered;

  long int getRawPos()
  {
    long int r;
    noInterrupts();
    r = raw_position;
    interrupts();
    return r;
  }

  RotationPositionWithLaps getPosition()
  {
    return RotationPositionWithLaps(this->getRawPos(), this->laps, this->position_remainder );
  }

  void handleOverflow()
  {
    int clicksPerRevInt = clicksPerRevolution.getInteger();
    SQ15x16 precise_position = 0;

    noInterrupts();
    if (raw_position > clicksPerRevInt)
    {
      precise_position = SQ15x16((long int)raw_position) + SQ15x16(position_remainder) - clicksPerRevolution;
      raw_position = floorFixed(precise_position).getInteger();
      position_remainder = SQ1x14(precise_position - floorFixed(precise_position));
      laps++;
    }
    interrupts();
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

    unsigned long int nowU = micros();

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
    noInterrupts();
    is_homed = false;
    pos_at_last_home = 0;
    interrupts();
  }

  void forceHomed()
  {
    noInterrupts();
    is_homed = true;
    pos_at_last_home = raw_position;
    raw_position = 0;    
    laps = 0;
    interrupts();  
  }

  bool isHomed()
  {
    noInterrupts();
    bool is_homed = is_homed;
    interrupts();
    return is_homed;
  }

  int getPosAtLastHome()
  {
    noInterrupts();
    int rValue = pos_at_last_home;
    interrupts();
    return rValue;
  }

  unsigned int getBreakerVal()
  {
    return digitalRead( breakerPin );
  }

};

//TODO: Convert HomingEncoder to a factory class that generates state objects.
class HomingEncoder
{

private:

public:
  HomingEncoderState state;
  static const SQ15x16 clicksPerRevolution;
  static HomingEncoderState *stateList[MAX_ENCODERS_SUPPORTED];
  const int speedTimeConstant = 10;

  template <int N>
  void config(unsigned int encoderPin1,
              unsigned int encoderPin2, unsigned int breakerPin,
              int offset)
  {
    pinMode(encoderPin1, INPUT_PULLUP);
    pinMode(encoderPin2, INPUT_PULLUP);
    pinMode(breakerPin, INPUT_PULLUP);

    state.encoderPin1 = encoderPin1;
    state.encoderPin2 = encoderPin2;
    state.breakerPin = breakerPin;

    state.raw_position = 0;

    state.is_homed = false;
    state.pos_at_last_home = 0;
    state.last_position = 0;
    state.last_position_timestamp_micros = 0;
    state.speed_cps = 0;

    // allow time for a passive R-C filter to charge
    // through the pullup resistors, before reading
    // the initial state
    delayMicroseconds(2000);

    static_assert(N < MAX_ENCODERS_SUPPORTED);

    stateList[N] = &state;

    attachInterrupt(digitalPinToInterrupt(encoderPin1), isr_encoder<N>, CHANGE);
    attachInterrupt(digitalPinToInterrupt(encoderPin2), isr_encoder<N>, CHANGE);
    attachInterrupt(digitalPinToInterrupt(breakerPin), isr_homing<N>, RISING); //Only trigger on rising or we home twice per rotation
  }

  //Should be called once every 10ms to compute the speed.
  virtual void run()
  {
    state.run();
  }

  long int getSpeedCPS()
  {
    return state.getSpeedCPS();
  }

  long int getSpeedCPSFiltered()
  {
    return state.getSpeedCPSFiltered();
  }

  RotationPositionWithLaps getPosition()
  {
    return state.getPosition();
  }

  void unHome()
  {
    state.unHome();
  }

  void forceHomed()
  {
    state.forceHomed();
  }

  bool isHomed()
  {
    return state.isHomed();
  }

  int getPosAtLastHome()
  {
    return state.getPosAtLastHome();
  }

  unsigned int getBreakerVal()
  {
    return state.getBreakerVal();
  }

public:
  template <int N>
  static void isr_encoder(void)
  {
    HomingEncoderState *state = stateList[N];
    state->raw_position++;
  }

  template <int N>
  static void isr_homing(void)
  {
    HomingEncoderState *state = stateList[N];
    if (!state->is_homed && state->raw_position > 200) //Do some debouncing
    { 
      state->is_homed = true;
      state->pos_at_last_home = state->raw_position;
      state->raw_position = 0;
    }
  }
  
};

#endif
