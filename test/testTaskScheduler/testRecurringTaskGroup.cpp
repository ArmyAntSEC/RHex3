#include <unity.h>
#define private public
#define protected public

#include <RecurringTaskGroup.h>
#include "TaskRunCounter.h"

void testCreateGroup()
{
    RecurringTaskGroup<5> group;
    
    TEST_ASSERT_EQUAL( 1000, group.rate ); //Run once per second as default.
}

void testCreateGroupWithArguments()
{
    RecurringTaskGroup<5> group(100);
    
    TEST_ASSERT_EQUAL( 100, group.rate );
}

void testGroupAdd()
{
    RecurringTaskGroup<1> group;
    Task task1;

    group.add( &task1 );

    TEST_ASSERT_EQUAL( 1, group.numTasks );
    TEST_ASSERT_EQUAL( &task1, group.taskList[0] );    
}

void testGroupAddTooMany()
{
    RecurringTaskGroup<2> group;
    Task task1;
    Task task2;
    Task task3;

    group.add( &task1 );
    group.add( &task2 );
    group.add( &task3 );

    TEST_ASSERT_EQUAL( 2, group.numTasks );
    TEST_ASSERT_EQUAL( &task1, group.taskList[0] );
    TEST_ASSERT_EQUAL( &task2, group.taskList[1] );    
}

void testGroupRun()
{
    RecurringTaskGroup<2> group(100);
    TaskRunCounter task1;
    TaskRunCounter task2;
    group.add( &task1 );
    group.add( &task2 );

    group.run(100);
    group.run(200);
    
    TEST_ASSERT_EQUAL( 200, group.runTime );
    
    TEST_ASSERT_EQUAL ( 2, task1.runCounter );
    TEST_ASSERT_EQUAL ( 2, task1.canRunCounter );

    TEST_ASSERT_EQUAL ( 2, task2.runCounter );
    TEST_ASSERT_EQUAL ( 2, task2.canRunCounter );
}

void processRecurringTaskGroup()
{
    RUN_TEST( testCreateGroup );
    RUN_TEST( testCreateGroupWithArguments );
    RUN_TEST( testGroupAdd );
    RUN_TEST( testGroupAddTooMany );
    RUN_TEST( testGroupRun );
}