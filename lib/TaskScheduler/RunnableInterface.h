#pragma once

struct RunnableInterface
{
    virtual void run(unsigned long int nowMicros) = 0;
};

struct RunnableAtTimeInterface
{
    virtual void run(unsigned long now) = 0;
    virtual bool canRun( unsigned long now) = 0;
};
