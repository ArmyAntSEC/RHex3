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

#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#define MAX_ENCODERS_SUPPORTED 6

struct HomingEncoderState {
  int encoderPin1;
  int encoderPin2;
  int breakerPin;
  
  volatile uint8_t * encoderPin1_register;
  volatile uint8_t * encoderPin2_register;
  volatile uint8_t * breakerPin_register;
  
  uint8_t encoderPin1_bitmask;
  uint8_t encoderPin2_bitmask;
  uint8_t breakerPin_bitmask;
  
  uint8_t encoder_state;

  volatile long int raw_position;
  long int laps;

  SQ1x14 position_remainder;
  
  volatile bool is_homed;  
  volatile long int pos_at_last_home;
  
  //int offset;

  long int last_position;
  unsigned long int last_position_timestamp_micros;
  SQ15x16 speed_cps;

};

class HomingEncoder
{
public:
  HomingEncoderState state;        
  
public:
  static const SQ15x16 clicksPerRevolution;
  static HomingEncoderState * stateList[MAX_ENCODERS_SUPPORTED];                
  
  template <int N> void config( unsigned int encoderPin1, 
			      unsigned int encoderPin2, unsigned int breakerPin,
			      int offset ) {
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
    
    /*
    state.encoderPin1_register = PIN_TO_BASEREG(encoderPin1);
    state.encoderPin2_register = PIN_TO_BASEREG(encoderPin2);
    state.breakerPin_register = PIN_TO_BASEREG(breakerPin);
    
    state.encoderPin1_bitmask = PIN_TO_BITMASK(encoderPin1);
    state.encoderPin2_bitmask = PIN_TO_BITMASK(encoderPin2);
    state.breakerPin_bitmask = PIN_TO_BITMASK(breakerPin);
    
    state.offset = offset;
    */

    // allow time for a passive R-C filter to charge
    // through the pullup resistors, before reading
    // the initial state
    delayMicroseconds(2000);
        
    static_assert( N < MAX_ENCODERS_SUPPORTED  );
    
    stateList[N] = &state;
    
    attachInterrupt(digitalPinToInterrupt(encoderPin1), isr_encoder<N>, CHANGE );
    attachInterrupt(digitalPinToInterrupt(encoderPin2), isr_encoder<N>, CHANGE );
    attachInterrupt(digitalPinToInterrupt(breakerPin), isr_homing<N>, RISING );  //Only trigger on rising or we home twice per rotation                   
    
  }
  
  //Should be called once every 10ms to compute the speed.
  virtual void run()
  {    
    
    //Max rpm: 3
    //Max clicks per second: 3*3500 = 10000
    //Max clicks per 0.01 s = 100

    //Min clicks per second 4700
    //Min clicks per 0.01s = 47
    
    unsigned long int nowU = micros();

    int thisPos = this->getRawPos();
    noInterrupts();
    int lastPos = state.last_position;
    unsigned long int lastTimeU = state.last_position_timestamp_micros;
    interrupts();

    SQ15x16 posDelta = HomingEncoder::positionPositiveDifference( thisPos, lastPos );
    state.last_position = thisPos;

    SQ15x16 timeDelta = nowU - lastTimeU;
    state.last_position_timestamp_micros = nowU;

    SQ15x16 speedCPS;
    if ( timeDelta == 0 ) {
      speedCPS = 0;
    } else {            
      //First pre-scale by 100, then divide, then scale by 10k for a total of 1e6.
      speedCPS = 10000*((100*posDelta)/timeDelta);
    }    
        
    state.last_position = thisPos;
    state.last_position_timestamp_micros = nowU;
    state.speed_cps = speedCPS;
        
    //Make sure that we handle any overflows.
    noInterrupts();
    HomingEncoder::handleOverflow( );
    interrupts();
    
    /*
    if ( speedCPS < 0 ) {
      ERROR( F("Unexpected speed") );
      ERROR( F("speedCPS:") << speedCPS.getInteger() );
      ERROR( F("Pos delta: ") << posDelta.getInteger() );
      ERROR( F("Time delta: ") << timeDelta.getInteger() );
      ERROR( F("This pos: ") << thisPos );
      ERROR ( F("Last pos:") << lastPos );
    }
    */

  }

  long int getSpeedCPS()
  {                 
    return state.speed_cps.getInteger();
  }

  long int getRawPos()
  {             
    long int r;              
    noInterrupts();
    r = state.raw_position;
    interrupts();    
    return r;
  }

  long int getPosComp()
  {             
    long int r;  
    /*                      
    noInterrupts();
    r = state.raw_position + state.offset;
    interrupts();
    */
    return r;
  }

  long int getPosSinceLastHoming()
  {             
    long int r; 
    /*                       
    noInterrupts();
    r = state.raw_position - state.pos_at_last_home;
    interrupts();
    */
    return r;
  }
  
  void unHome()
  {    
    noInterrupts();    
    state.is_homed = false;
    state.pos_at_last_home = 0;
    interrupts();   
  }
  
  void forceHomed()
  {   
    noInterrupts();
    state.is_homed = true;
    state.pos_at_last_home = state.raw_position;
    state.raw_position = 0;    
    state.laps = 0;
    interrupts();   
  }

  bool isHomed() 
  {     
    noInterrupts();
    bool is_homed = state.is_homed;
    interrupts();    
    return is_homed;
  }

  
  int getPosAtLastHome()
  {    
    noInterrupts();
    int rValue = state.pos_at_last_home;
    interrupts();    
    return rValue;
  }

  int getLaps()
  {
        
    noInterrupts();
    int rValue = state.laps;
    interrupts();    
    return rValue;
  }

  unsigned int getBreakerVal()
  {
    return DIRECT_PIN_READ(state.breakerPin_register, 
      state.breakerPin_bitmask );                           
  }
  
public:
  template<int N> static void isr_encoder(void) 
  {
    HomingEncoderState * state = stateList[N];    
    state->raw_position++; 
  }
  
  template<int N> static void isr_homing(void) 
    { 
      HomingEncoderState * state = stateList[N];
      if ( !state->is_homed && state->raw_position > 200 ) { //Do some debouncing
        state->is_homed = true;
        state->pos_at_last_home = state->raw_position;
        state->raw_position = 0;
      }
      /*
      uint8_t breaker_val = DIRECT_PIN_READ(state->breakerPin_register, 
        state->breakerPin_bitmask );                           
      #if defined (ARDUINO_AVR_UNO)
        //A bit of an ugly hack, but it allows u to call this function as often as we want on the Uno, but it only
        //does anything if the breaker has actually toggled

        if ( breaker_val == state->homing_pin_last_value )
	        return;
        else
	        state->homing_pin_last_value = breaker_val; 
      #endif
      
      //Depending on direction, we will trigger either on rising or falling. 
      //We want to make sure we allways trigger on the same edge regardless of direction.
      //If we are allready homed, do not rehome if we have not moved at least 2500 steps. 
      //This to avoid bouncing of the signal as the homing is happening.
      if ( !state->is_homed && state->moving_forward ^ breaker_val ) {                                
        state->pos_at_last_home = state->raw_position;
        state->raw_position = 0;                 
        state->is_homed = true;                      
      }      
      */

    }

    void handleOverflow ( )
    {                        
      int clicksPerRevInt = clicksPerRevolution.getInteger();
      SQ15x16 precise_position = 0;

      noInterrupts();
      if ( state.raw_position > clicksPerRevInt ) {                        
        precise_position = SQ15x16((long int)state.raw_position) + SQ15x16(state.position_remainder) - HomingEncoder::clicksPerRevolution;
        state.raw_position = floorFixed(precise_position).getInteger();
        state.position_remainder = SQ1x14(precise_position-floorFixed(precise_position));
        state.laps++;
      }              
      interrupts();    
    }

    static long int positionPositiveDifference( long int pos1, long int pos2 )
    {
      if ( pos1 > pos2 )
        return pos1 - pos2;
      else
        return HomingEncoder::clicksPerRevolution.getInteger() + pos1 - pos2;
    }    
};

#endif
