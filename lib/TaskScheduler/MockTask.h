#pragma once

#include "Task.h"

struct MockTask: public Task
{
    
    int countCanRun = 0;
    int countRun = 0;
    bool canActuallyRun = true; 
    
    virtual bool canRun(unsigned long int now) {
        countCanRun++;
        return canActuallyRun;
    }

    virtual void run(unsigned long int now)
    {
        countRun++;
    }
};