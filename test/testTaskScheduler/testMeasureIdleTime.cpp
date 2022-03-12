#include <unityEx.h>
#include <IdleCounter.h>

void testShallCountIfNothingRan()
{
    IdleCounter sut;
    TEST_ASSERT_EQUAL( 0, sut.getIdleCounter() );    
    sut.SignelOneCycleRunAndResetTaskRunStatus();
    TEST_ASSERT_EQUAL( 1, sut.getIdleCounter() );
}

void testShallNotCountIfSomethingRan()
{
    IdleCounter sut;
    TEST_ASSERT_EQUAL( 0, sut.getIdleCounter() );    
    sut.SignalOneTaskWasRun();
    sut.SignelOneCycleRunAndResetTaskRunStatus();
    TEST_ASSERT_EQUAL( 0, sut.getIdleCounter() );
}

void testShallResetTaskStatusOnEachCycle()
{
    IdleCounter sut;
    TEST_ASSERT_EQUAL( 0, sut.getIdleCounter() );    
    sut.SignalOneTaskWasRun();
    sut.SignelOneCycleRunAndResetTaskRunStatus();
    sut.SignelOneCycleRunAndResetTaskRunStatus();
    TEST_ASSERT_EQUAL( 1, sut.getIdleCounter() );
}

void runAllTestsMeasureIdleTime()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testShallCountIfNothingRan );
    RUN_TEST( testShallNotCountIfSomethingRan );
    RUN_TEST( testShallResetTaskStatusOnEachCycle );
    UNITY_END_INT();
}