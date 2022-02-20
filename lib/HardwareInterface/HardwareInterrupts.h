#pragma once

#ifdef ARDUINO

#include <Arduino.h>

class HardwareInterrupts
{
public:
    void enableInterrupts()
    {
        interrupts();
    }
    void disableInterrupts()
    {
        noInterrupts();
    }
};

#else

class HardwareInterrupts
{
public:
    void enableInterrupts()
    {
        //Test context, so no iterrupts.
    }
    void disableInterrupts()
    {
        //Test context, so no interrupts
    }
};

#endif