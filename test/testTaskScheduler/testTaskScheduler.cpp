#include <unity.h>
#define private public

#include <TaskScheduler.h>
#include <MockTask.h>

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

    scheduler.run();

    TEST_ASSERT_EQUAL( 1, task1.countCanRun );
    TEST_ASSERT_EQUAL( 1, task2.countCanRun );    
    TEST_ASSERT_EQUAL( 0, task1.countRun );
    TEST_ASSERT_EQUAL( 1, task2.countRun );    

    task1.canActuallyRun = true;
    scheduler.run();

    TEST_ASSERT_EQUAL( 2, task1.countCanRun );
    TEST_ASSERT_EQUAL( 2, task2.countCanRun );    
    TEST_ASSERT_EQUAL( 1, task1.countRun );
    TEST_ASSERT_EQUAL( 2, task2.countRun );    
}

void processTaskScheduler()
{
    RUN_TEST( testAddTask );
    RUN_TEST( testRun );
}