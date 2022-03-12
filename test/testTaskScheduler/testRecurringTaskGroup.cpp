#include <unityEx.h>

#define private public

#include <RecurringTaskGroup.h>

struct MockRunnable: public RunnableInterface
{
    int runCount = 0;
    virtual void run(unsigned long int now)
    {
        runCount++;
    }
};

void testAddRecurringTaskToGroup()
{        
    RecurringTaskGroup<3> group;
    RunnableInterface* task;
    group.addTask( task );

    TEST_ASSERT_EQUAL( 1, group.getNumberOfTasks() );
    TEST_ASSERT_EQUAL( task, group.getTask(0) );
} 

void testRunRecurringTasks()
{
    RecurringTaskGroup<3> group;
    MockRunnable task;
    MockRunnable task2;
    group.addTask( &task );
    group.addTask( &task2 );

    group.run( 0 );

    TEST_ASSERT_EQUAL( 1, task.runCount );
    TEST_ASSERT_EQUAL( 1, task2.runCount );
}

void testCanRun()
{
    RecurringTaskGroup<3> group( 1000*1000L );
    group.nextRunTimeMicros = 1000*1000L;
    TEST_ASSERT_TRUE( group.canRun(1500*1000L) );
    TEST_ASSERT_EQUAL( 1000*1000L, group.nextRunTimeMicros ); //CanRun should not change nextRunTime.

    group.nextRunTimeMicros = 2000*1000L;
    TEST_ASSERT_FALSE( group.canRun(1500*1000L) );
}

void testRunAndIncrementsNextTime()
{
    RecurringTaskGroup<3> group( 1000*1000L );
    group.nextRunTimeMicros = 1000*1000L;
    
    unsigned long thisTime = 1500*1000L;

    TEST_ASSERT_TRUE( group.canRun(thisTime) );
    group.run(thisTime);    
    TEST_ASSERT_FALSE( group.canRun(thisTime) );
}

void runAllRecurringTaskGroupTests()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testAddRecurringTaskToGroup );
    RUN_TEST( testRunRecurringTasks );
    RUN_TEST( testCanRun );
    RUN_TEST( testRunAndIncrementsNextTime );
    UNITY_END_INT();
}
