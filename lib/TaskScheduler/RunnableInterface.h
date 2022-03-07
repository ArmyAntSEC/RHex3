#pragma once

struct RunnableInterface
{
    virtual void run(unsigned long int nowMicros) = 0;
};

struct RunnableAtTimeInterface: public RunnableInterface
{    
    virtual bool canRun( unsigned long now) = 0;
};

