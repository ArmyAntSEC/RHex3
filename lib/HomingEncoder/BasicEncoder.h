#pragma once

#include <HardwarePins.h>
#include "HomingEncoderInterfaces.h"

#define MAX_ENCODERS_SUPPORTED 6

class BasicEncoder
{
private:
    int16_t encoderPin1;
    int16_t encoderPin2;
    int16_t homingPin;
    BasicEncoderListener* listeners[2];        
    int16_t listenerCount;          

    BasicEncoder()
    {}

public:
    void config(uint16_t _encoderPin1, uint16_t _encoderPin2, uint16_t _breakerPin, 
                HardwarePinsInterface* _pins )
    {
        encoderPin1 = _encoderPin1;
        encoderPin2 = _encoderPin2;
        homingPin = _breakerPin;        

        _pins->configurePin(encoderPin1, _pins->INPUT_PULLUP);
        _pins->configurePin(encoderPin2, _pins->INPUT_PULLUP);
        _pins->configurePin(homingPin, _pins->INPUT_PULLUP);

        listenerCount = 0;          
    }

    void addListener( BasicEncoderListener* _listener )
    {        
        this->listeners[listenerCount] = _listener;                
        listenerCount++;
    }

    void signalStepForwardISR()
    {        
        for ( int16_t i = 0; i < listenerCount; i++ ) {
            listeners[i]->signalStepForwardISR();
        }
    }

    void signalHomingISR()
    {
        for ( int16_t i = 0; i < listenerCount; i++ ) {
            listeners[i]->signalHomingISR();
        }
    }

    friend class BasicEncoderFactory;
};

class BasicEncoderFactory
{
public:
    static BasicEncoder stateList[MAX_ENCODERS_SUPPORTED];

    template <int16_t N>
    static BasicEncoder *config(
        uint16_t encoderPin1, uint16_t encoderPin2,
        uint16_t homingPin, HardwarePinsInterface* pins )
    {
        static_assert(N < MAX_ENCODERS_SUPPORTED, "Too many encoders requested");
        BasicEncoder *state = &stateList[N];

        state->config(encoderPin1, encoderPin2, homingPin, pins);        

        // Only trigger homing on rising or we home twice per rotation
        pins->attachAnInterrupt( homingPin, isr_homing<N>, pins->FALLING );
        pins->attachAnInterrupt( encoderPin1, isr_encoder<N>, pins->CHANGE );
        
        return state;
    }

    template <int16_t N>
    static void isr_encoder(void)
    {
        BasicEncoder *state = &stateList[N];
        state->signalStepForwardISR();
    }

    template <int16_t N>
    static void isr_homing(void)
    {
        BasicEncoder *state = &stateList[N];
        state->signalHomingISR();
    }
};
