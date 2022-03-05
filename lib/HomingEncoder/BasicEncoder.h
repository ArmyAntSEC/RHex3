#pragma once

#include <HardwarePins.h>
#include "HomingEncoderInterfaces.h"

#define MAX_ENCODERS_SUPPORTED 6

class BasicEncoder
{
private:
    int encoderPin1;
    int encoderPin2;
    int homingPin;
    BasicEncoderListener* listeners[2];        
    int listenerCount;          

    BasicEncoder()
    {}

public:
    void config(unsigned int _encoderPin1,
                unsigned int _encoderPin2, unsigned int _breakerPin, 
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
        for ( int i = 0; i < listenerCount; i++ ) {
            listeners[i]->signalStepForwardISR();
        }
    }

    void signalHomingISR()
    {
        for ( int i = 0; i < listenerCount; i++ ) {
            listeners[i]->signalHomingISR();
        }
    }

    friend class BasicEncoderFactory;
};

class BasicEncoderFactory
{
public:
    static BasicEncoder stateList[MAX_ENCODERS_SUPPORTED];

    template <int N>
    static BasicEncoder *config(
        unsigned int encoderPin1, unsigned int encoderPin2,
        unsigned int homingPin, HardwarePinsInterface* pins )
    {
        static_assert(N < MAX_ENCODERS_SUPPORTED, "Too many encoders requested");
        BasicEncoder *state = &stateList[N];

        state->config(encoderPin1, encoderPin2, homingPin, pins);        

        // Only trigger homing on rising or we home twice per rotation
        pins->attachAnInterrupt( homingPin, isr_homing<N>, pins->FALLING );
        pins->attachAnInterrupt( encoderPin1, isr_encoder<N>, pins->CHANGE );
        
        return state;
    }

    template <int N>
    static void isr_encoder(void)
    {
        BasicEncoder *state = &stateList[N];
        state->signalStepForwardISR();
    }

    template <int N>
    static void isr_homing(void)
    {
        BasicEncoder *state = &stateList[N];
        state->signalHomingISR();
    }
};
