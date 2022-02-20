#include <unity.h>

#define private public
#include <TaskScheduler2.h>

struct RunnableAtTimeMock: public RunnableAtTime
{
    int runCount = 0;
    unsigned long lastRunTime = 0;
    unsigned long nextRun = 1000;

    virtual void run(unsigned long now)
    {
        runCount++;
        lastRunTime = now;
    }

    virtual bool canRun( unsigned long now)
    {
        return now > nextRun;
    }

};

void testAddTask()
{
    TaskScheduler<6> scheduler;
    RunnableAtTime* task;
    RunnableAtTime* task2;
    
    scheduler.addTask( task );
    scheduler.addTask( task2 );

    TEST_ASSERT_EQUAL( 2, scheduler.getNumberOfTasks() );
    TEST_ASSERT_EQUAL( task, scheduler.getTask(0) );
    TEST_ASSERT_EQUAL( task2, scheduler.getTask(1) );
}

void testRunTasksAfterTime()
{
    TaskScheduler<6> scheduler;
    RunnableAtTimeMock task;   
    RunnableAtTimeMock task2;    
    scheduler.addTask( &task );
    scheduler.addTask( &task2 );

    scheduler.run( 1500 );

    TEST_ASSERT_EQUAL( 1, task.runCount );
    TEST_ASSERT_EQUAL( 1500, task.lastRunTime );
    TEST_ASSERT_EQUAL( 1, task2.runCount );
    TEST_ASSERT_EQUAL( 1500, task2.lastRunTime );   
}

void testRunTasksBeforeTime()
{
    TaskScheduler<6> scheduler;
    RunnableAtTimeMock task;   
    RunnableAtTimeMock task2;  
    task2.nextRun = 2000;  
    scheduler.addTask( &task );
    scheduler.addTask( &task2 );

    scheduler.run( 1500 );

    TEST_ASSERT_EQUAL( 1, task.runCount );    
    TEST_ASSERT_EQUAL( 0, task2.runCount );    
}

void testDelayWithScheduler()
{
    TaskScheduler<6> scheduler;    
}

void runAllTaskSchedulerTests()
{
    UNITY_BEGIN();
    RUN_TEST( testAddTask );
    RUN_TEST( testRunTasksAfterTime );
    RUN_TEST( testRunTasksBeforeTime );
    UNITY_END();
}