#pragma once

struct RunnableInterface
{
    virtual void run(uint32_t nowMicros) = 0;
};

struct RunnableAtTimeInterface: public RunnableInterface
{    
    virtual bool canRun( uint32_t now) = 0;
};

