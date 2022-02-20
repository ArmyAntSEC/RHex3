#pragma once

struct Runnable
{
    virtual void run(unsigned long int nowMicros) = 0;
};
