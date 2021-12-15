#pragma once

#include <Task.h>

struct TaskRunCounter: public Task
{
    int runCounter = 0;
    int canRunCounter = 0;

    virtual bool canRun(unsigned long now)
    {
        canRunCounter++;
        return Task::canRun(now);
    }

    virtual void run(unsigned long now)
    {
        runCounter++;
    }        

};