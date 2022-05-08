#pragma once

struct RunnableInterface
{
    virtual void run(int32_t nowMicros) = 0;
};

struct RunnableAtTimeInterface : public RunnableInterface
{
    virtual bool canRun(int32_t now) = 0;
};

struct TaskAwareDelayInterface
{
    virtual void delayMicros(int32_t micros) = 0;
};