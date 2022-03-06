#pragma once

struct HardwareInterruptsInterface
{
    virtual void enableInterrupts() = 0;    
    virtual void disableInterrupts() = 0;    
};

#ifdef ARDUINO

#include <Arduino.h>

class HardwareInterrupts: public HardwareInterruptsInterface
{
public:
    virtual void enableInterrupts()
    {
        interrupts();
    }
    virtual void disableInterrupts()
    {
        noInterrupts();
    }
};

#else

class HardwareInterruptsMock: public HardwareInterruptsInterface
{
public:
    int interruptsEnabledCount = 0;
    int interruptsDisabledCount = 0;

    void enableInterrupts()
    {
        interruptsEnabledCount++;
    }
    
    void disableInterrupts()
    {
        interruptsDisabledCount++;
    }
};

#endif