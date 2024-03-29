#include <unityEx.h>

#define private public
#include <TaskScheduler.h>
#include <IdleCounter.h>

struct RunnableAtTimeMock: public RunnableAtTimeInterface
{
    int16_t runCount = 0;
    int32_t lastRunTime = 0;
    int32_t nextRun = 1000;

    virtual void run(int32_t now)
    {
        runCount++;
        lastRunTime = now;
    }

    virtual bool canRun( int32_t now)
    {
        return now > nextRun;
    }

};

void testAddTask()
{
    HardwareClockMock hwClock;
    TaskScheduler scheduler(&hwClock);

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
    HardwareClockMock hwClock;
    TaskScheduler scheduler(&hwClock);    
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
    HardwareClockMock hwClock;
    TaskScheduler scheduler(&hwClock);
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
    HardwareClockMock hwClock;
    TaskScheduler scheduler(&hwClock);
    IdleCounter* idleCounter = scheduler.getIdleCounterObject();    
    RunnableAtTimeMock task;   
    scheduler.addTask( &task );
    task.lastRunTime = 1000;

    //Increment idle count when not running a task.
    scheduler.run( 500 );
    TEST_ASSERT_EQUAL( 1, idleCounter->getIdleCounter() );

    //Do not increment idle count when running task.
    scheduler.run( 1500 );
    TEST_ASSERT_EQUAL( 1, idleCounter->getIdleCounter() );
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