#include <unityEx.h>

#define private public
#include <TaskScheduler.h>
#include <IdleCounter.h>

struct RunnableAtTimeMock: public RunnableAtTimeInterface
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
    TaskScheduler scheduler;
    RunnableAtTimeInterface* task;
    RunnableAtTimeInterface* task2;
    
    scheduler.addTask( task );
    scheduler.addTask( task2 );

    TEST_ASSERT_EQUAL( 2, scheduler.getNumberOfTasks() );
    TEST_ASSERT_EQUAL( task, scheduler.getTask(0) );
    TEST_ASSERT_EQUAL( task2, scheduler.getTask(1) );
}

void testRunTasksAfterTime()
{
    TaskScheduler scheduler;
    IdleCounter idleCounter;
    scheduler.setIdleCounter( &idleCounter );
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
    TaskScheduler scheduler;
    IdleCounter idleCounter;
    scheduler.setIdleCounter( &idleCounter );
    RunnableAtTimeMock task;   
    RunnableAtTimeMock task2;  
    task2.nextRun = 2000;  
    scheduler.addTask( &task );
    scheduler.addTask( &task2 );

    scheduler.run( 1500 );

    TEST_ASSERT_EQUAL( 1, task.runCount );    
    TEST_ASSERT_EQUAL( 0, task2.runCount );    
}

void testMeasureIdleTime()
{
    TaskScheduler scheduler;
    IdleCounter idleCounter;
    scheduler.setIdleCounter( &idleCounter );
    RunnableAtTimeMock task;   
    scheduler.addTask( &task );
    task.lastRunTime = 1000;

    //Increment idle count when not running a task.
    scheduler.run( 500 );
    TEST_ASSERT_EQUAL( 1, idleCounter.getIdleCounter() );

    //Do not increment idle count when running task.
    scheduler.run( 1500 );
    TEST_ASSERT_EQUAL( 1, idleCounter.getIdleCounter() );
}

void runAllTaskSchedulerTests()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testAddTask );
    RUN_TEST( testRunTasksAfterTime );
    RUN_TEST( testRunTasksBeforeTime );
    RUN_TEST( testMeasureIdleTime );
    UNITY_END_INT();
}