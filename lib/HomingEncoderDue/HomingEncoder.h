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

#if defined(ARDUINO_SAM_DUE)
  #define IO_REG_TYPE			uint32_t
#elif defined(ARDUINO_AVR_UNO)
  #define IO_REG_TYPE			uint8_t
#endif

#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#define MAX_ENCODERS_SUPPORTED 6

struct HomingEncoderState {
  int encoderPin1;
  int encoderPin2;
  int breakerPin;
  
  volatile IO_REG_TYPE * encoderPin1_register;
  volatile IO_REG_TYPE * encoderPin2_register;
  volatile IO_REG_TYPE * breakerPin_register;
  
  IO_REG_TYPE encoderPin1_bitmask;
  IO_REG_TYPE encoderPin2_bitmask;
  IO_REG_TYPE breakerPin_bitmask;
  
  uint8_t encoder_state;

  bool moving_forward;
  
  int position;
  int rotations;
  
  bool is_homed;
  
  int pos_at_last_home;
  
  int offset;

  int last_position;
  unsigned long int last_position_timestamp_micros;
  int speed_cps;

#if defined (ARDUINO_AVR_UNO)
  uint8_t homing_pin_last_value; //Only used for Uno to software emulate the homing pin interrupt
#endif
};

class HomingEncoder
{
private:
  HomingEncoderState state;        
  
public:
  static int clicksPerRevolution()
  {
    return 3592;
  }

  static HomingEncoderState * stateList[MAX_ENCODERS_SUPPORTED];                
  
  template <int N> void init( unsigned int encoderPin1, 
			      unsigned int encoderPin2, unsigned int breakerPin,
			      int offset ) {
    pinMode(encoderPin1, INPUT_PULLUP);
    pinMode(encoderPin2, INPUT_PULLUP);
    pinMode(breakerPin, INPUT_PULLUP);
    
    state.encoderPin1 = encoderPin1;
    state.encoderPin2 = encoderPin2;
    state.breakerPin = breakerPin;
    state.position = 0;
    state.rotations = 0;
    state.moving_forward = true;
    state.is_homed = false;
    state.pos_at_last_home = 0;
    state.last_position = 0;
    state.last_position_timestamp_micros = 0;
    state.speed_cps = 0;        
    
    state.encoderPin1_register = PIN_TO_BASEREG(encoderPin1);
    state.encoderPin2_register = PIN_TO_BASEREG(encoderPin2);
    state.breakerPin_register = PIN_TO_BASEREG(breakerPin);
    
    state.encoderPin1_bitmask = PIN_TO_BITMASK(encoderPin1);
    state.encoderPin2_bitmask = PIN_TO_BITMASK(encoderPin2);
    state.breakerPin_bitmask = PIN_TO_BITMASK(breakerPin);
    
    state.offset = offset;
    
    // allow time for a passive R-C filter to charge
    // through the pullup resistors, before reading
    // the initial state
    delayMicroseconds(2000);
    
    uint8_t s = 0;
    if (DIRECT_PIN_READ(state.encoderPin1_register, state.encoderPin1_bitmask)) s |= 1;
    if (DIRECT_PIN_READ(state.encoderPin1_register, state.encoderPin1_bitmask)) s |= 2;

#if defined (ARDUINO_AVR_UNO)
    state.homing_pin_last_value = DIRECT_PIN_READ(state.breakerPin_register, state.breakerPin_bitmask);
#endif
													 												       
    state.encoder_state = s;            
    
    if ( N < MAX_ENCODERS_SUPPORTED ) {                            
      stateList[N] = &state;
      
      attachInterrupt(digitalPinToInterrupt(encoderPin1), isr_encoder<N>, CHANGE );
      attachInterrupt(digitalPinToInterrupt(encoderPin2), isr_encoder<N>, CHANGE );
#if defined(ARDUINO_SAM_DUE)
      attachInterrupt(digitalPinToInterrupt(breakerPin), isr_homing<N>, CHANGE );                     
#endif      
    }
  }

  virtual void run( unsigned long int now_micro )
  {    
    int thisPos = this->readCompensatedPos();
    int posDelta = thisPos - state.last_position;
    state.last_position = thisPos;

    int timeDelta = now_micro - state.last_position_timestamp_micros;

    if ( timeDelta == 0 )
      state.speed_cps = 0;
    else
      state.speed_cps = (1e6*posDelta)/timeDelta;
  }

  int readSpeed()
  {
    int speedCPS = 0;
    noInterrupts();
    speedCPS = state.speed_cps;
    interrupts();  
    return speedCPS;
  }

  int readCompensatedPos()
    {             
      int r;                        
      noInterrupts();
      r = state.position + state.offset;
      interrupts();
      return r;
    }
  
  void unHome()
    {
      noInterrupts();
      state.is_homed = false;
      state.pos_at_last_home = 0;
      interrupts();
    }
  
  bool isHomed() 
    {
      bool is_homed;
      noInterrupts();
      is_homed = state.is_homed;
      interrupts();
      return is_homed;
    }
  
  int getRotations()
    {
      int rotations;
      noInterrupts();
      rotations = state.rotations;
      interrupts();
      return rotations;
    }
  
  int getPosAtLastHome()
    {
      int rValue = 0;
      noInterrupts();
      rValue = state.pos_at_last_home;
      interrupts();
      return rValue;
    }
  
public:
  template<int N> static void isr_encoder(void) 
    {
      HomingEncoderState * state = stateList[N];
      
      uint8_t p1val = DIRECT_PIN_READ(state->encoderPin1_register, 
				      state->encoderPin1_bitmask );
      uint8_t p2val = DIRECT_PIN_READ(state->encoderPin2_register,
				      state->encoderPin2_bitmask );
      
      uint8_t encoder_state = state->encoder_state & 3;
      if (p1val) encoder_state |= 4;
      if (p2val) encoder_state |= 8;
      state->encoder_state = (encoder_state >> 2);
      
      switch (encoder_state) {
      case 1: case 7: case 8: case 14:
	state->position--;
	state->moving_forward = false;
	break;
      case 2: case 4: case 11: case 13:
	state->position++;
	state->moving_forward = true;
	break;
      case 3: case 12:
	state->position -= 2;
	state->moving_forward = false;
	break;
      case 6: case 9:
	state->position += 2;
	state->moving_forward = true;
	break;
      }                                    
    }
  
  template<int N> static void isr_homing(void) 
    { 
      HomingEncoderState * state = stateList[N];
      
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
      //We want to make sure we allways trigger on the same edge regardless of direction
      if ( abs(state->position) > 2500 && state->moving_forward ^ breaker_val ) {                                
	state->is_homed = true;
	state->pos_at_last_home = state->position;
	state->position = 0;
	if ( state->moving_forward )
	  state->rotations++;
	else
	  state->rotations--;
      }            
    }
};

#endif
