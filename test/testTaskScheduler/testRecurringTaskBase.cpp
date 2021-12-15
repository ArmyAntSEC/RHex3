#include <unity.h>
#define private public

#include <RecurringTaskBase.h>

void testCanRun()
{
    RecurringTaskBase task;
    task.running = true;

    TEST_ASSERT_EQUAL( true, task.canRun(0) );
}

void testInit()
{
    RecurringTaskBase task;
    task.init();

    TEST_ASSERT_EQUAL( true, task.isRunning() );
}

void testStart()
{
    RecurringTaskBase task;
    task.start();

    TEST_ASSERT_EQUAL( true, task.isRunning() );
}

void testStop()
{
    RecurringTaskBase task;
    task.stop();

    TEST_ASSERT_EQUAL( false, task.isRunning() );
}

void processRecurringTaskBase()
{
    RUN_TEST( testCanRun );
    RUN_TEST( testInit );
    RUN_TEST( testStart );
    RUN_TEST( testStop );
}