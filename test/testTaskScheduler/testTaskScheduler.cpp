#include <unity.h>
#define private public

#include <TaskScheduler.h>
#include <MockTask.h>
#include <HardwareInterface.h>

void testAddTask()
{
    TaskScheduler scheduler;
    MockTask task;
    scheduler.add( &task );

    TEST_ASSERT_EQUAL( 1, scheduler.numTasks );
    TEST_ASSERT_EQUAL( &task, scheduler.tasks[0] );    
}

void testRun()
{
    TaskScheduler scheduler;
    MockTask task1;
    task1.canActuallyRun = false;
    MockTask task2;
    task2.canActuallyRun = true;

    scheduler.add( &task1 );
    scheduler.add( &task2 );

    scheduler.run(0);

    TEST_ASSERT_EQUAL( 1, task1.countCanRun );
    TEST_ASSERT_EQUAL( 1, task2.countCanRun );    
    TEST_ASSERT_EQUAL( 0, task1.countRun );
    TEST_ASSERT_EQUAL( 1, task2.countRun );    

    task1.canActuallyRun = true;
    scheduler.run(10);

    TEST_ASSERT_EQUAL( 2, task1.countCanRun );
    TEST_ASSERT_EQUAL( 2, task2.countCanRun );    
    TEST_ASSERT_EQUAL( 1, task1.countRun );
    TEST_ASSERT_EQUAL( 2, task2.countRun );    
}

void testDelayWithoutStoppingScheduler()
{
    TaskScheduler scheduler;
    HardwareInterface::resetMicrosecondsSinceBoot();
    MockTask task;
    task.canActuallyRun = true;
    scheduler.add(&task);
    
    scheduler.delayWithoutStoppingScheduler( 100 );
    TEST_ASSERT_EQUAL( 110, HardwareInterface::getMicrosecondsSinceBoot()/1000 ); //There are 2 additional calls to getMillis()
    TEST_ASSERT_EQUAL( 10, task.countCanRun );
}

void processTaskScheduler()
{
    RUN_TEST( testAddTask );
    RUN_TEST( testRun );
    RUN_TEST( testDelayWithoutStoppingScheduler );
}