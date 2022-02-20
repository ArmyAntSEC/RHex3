#pragma once

struct RunnableInterface
{
    virtual void run(unsigned long int nowMicros) = 0;
};
