#include <unity.h>
#define private public

#include <SingleTask.h>

void testInit()
{
    //TODO: This file shall test REcurringTaskBase. REcurringTask can be mergesd into RecurringTaskGroup and then deleted.
}

void processRecurringTask()
{
    RUN_TEST( testInit );
}