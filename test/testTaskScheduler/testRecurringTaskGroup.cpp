#include <unity.h>

#define private public

#include <RecurringTaskGroup2.h>

struct MockRunnable: public Runnable
{
    int runCount = 0;
    virtual void run(unsigned long int now)
    {
        runCount++;
    }
};

void testAddTestToGroup()
{        
    RecurringTaskGroup<3> group;
    Runnable* task;
    group.addTask( task );

    TEST_ASSERT_EQUAL( 1, group.getNumberOfTasks() );
    TEST_ASSERT_EQUAL( task, group.getTask(0) );
} 

void testRunTasksAfterTime()
{
    RecurringTaskGroup<3> group;
    MockRunnable task;
    MockRunnable task2;
    group.addTask( &task );
    group.addTask( &task2 );

    group.run( 1500 );

    TEST_ASSERT_EQUAL( 1, task.runCount );
    TEST_ASSERT_EQUAL( 1, task2.runCount );
}

void testRunTasksTooEarly()
{
    RecurringTaskGroup<3> group(1000);
    MockRunnable task;    
    group.addTask( &task );    

    group.run( 100 );

    TEST_ASSERT_EQUAL( 0, task.runCount );    
}

void testRunTasksTwiceWithoutIncreasingTime()
{
    RecurringTaskGroup<3> group;
    MockRunnable task;
    MockRunnable task2;
    group.addTask( &task );
    group.addTask( &task2 );

    group.run( 1500 );
    group.run( 1500 );

    TEST_ASSERT_EQUAL( 1, task.runCount );
    TEST_ASSERT_EQUAL( 1, task2.runCount );
}

void processAllRecurringTaskGroupTests()
{
    UNITY_BEGIN();
    RUN_TEST( testAddTestToGroup );
    RUN_TEST( testRunTasksAfterTime );
    RUN_TEST( testRunTasksTooEarly );
    RUN_TEST( testRunTasksTwiceWithoutIncreasingTime );
    UNITY_END();    
}
