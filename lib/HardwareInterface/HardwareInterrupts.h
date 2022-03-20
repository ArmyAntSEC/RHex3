#pragma once
#include <base.h>

struct HardwareInterruptsInterface
{
    virtual void enableInterrupts() = 0;    
    virtual void disableInterrupts() = 0;    
};

#ifdef ARDUINO

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

#endif

class HardwareInterruptsMock: public HardwareInterruptsInterface
{
public:
    int16_t interruptsEnabledCount = 0;
    int16_t interruptsDisabledCount = 0;

    void enableInterrupts()
    {
        interruptsEnabledCount++;
    }
    
    void disableInterrupts()
    {
        interruptsDisabledCount++;
    }
};
