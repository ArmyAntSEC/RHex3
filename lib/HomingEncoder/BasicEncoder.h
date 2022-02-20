#pragma once

#include <HardwareInterface.h>
#include <HardwarePins.h>

#define MAX_ENCODERS_SUPPORTED 6

class BasicEncoderListener
{
public:
    virtual void signalStepForwardISR() = 0;
    virtual void signalHomingISR() = 0;
};

class BasicEncoder
{
private:
    int encoderPin1;
    int encoderPin2;
    int homingPin;
    BasicEncoderListener* listener;

    BasicEncoder()
    {}

public:
    void config(unsigned int _encoderPin1,
                unsigned int _encoderPin2, unsigned int _breakerPin, BasicEncoderListener* listener)
    {
        encoderPin1 = _encoderPin1;
        encoderPin2 = _encoderPin2;
        homingPin = _breakerPin;

        this->listener = listener;

        HardwareInterface::configurePin(encoderPin1, HardwareInterface::INPUT_PULLUP);
        HardwareInterface::configurePin(encoderPin2, HardwareInterface::INPUT_PULLUP);
        HardwareInterface::configurePin(homingPin, HardwareInterface::INPUT_PULLUP);
    }

    void signalStepForwardISR()
    {
        listener->signalStepForwardISR();
    }

    void signalHomingISR()
    {
        listener->signalHomingISR();
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
        unsigned int homingPin, BasicEncoderListener* listener )
    {
        static_assert(N < MAX_ENCODERS_SUPPORTED, "Too many encoders requested");
        BasicEncoder *state = &stateList[N];

        state->config(encoderPin1, encoderPin2, homingPin, listener);

        // Only trigger homing on rising or we home twice per rotation
        HardwareInterface::attachAnInterrupt(homingPin, isr_homing<N>, HardwareInterface::FALLING);
        HardwareInterface::attachAnInterrupt(encoderPin1, isr_encoder<N>, HardwareInterface::CHANGE);

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
