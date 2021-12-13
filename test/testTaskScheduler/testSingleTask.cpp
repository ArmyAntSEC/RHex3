#include <unity.h>
#define private public

#include <SingleTask.h>

void testInit()
{
    SingleTask task;
    task.init( 123 );

    TEST_ASSERT_EQUAL( 123, task.runTime );    
}

void testCanRun()
{
    SingleTask task;
    task.init( 123 );

    TEST_ASSERT_TRUE( task.canRun( 124 ) );
    TEST_ASSERT_FALSE( task.canRun( 122 ) );    
}

void processSingleTask()
{
    RUN_TEST( testInit );
    RUN_TEST( testCanRun );
}