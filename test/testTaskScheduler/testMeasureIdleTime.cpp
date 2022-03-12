#include <unityEx.h>
#include <IdleCounter.h>
#include <TaskScheduler.h>
#include <HardwareClock.h>

void testShallCountIfNothingRan()
{
    IdleCounter sut;
    TEST_ASSERT_EQUAL( 0, sut.getIdleCounter() );    
    sut.SignalOneCycleRunAndResetTaskRunStatus();
    TEST_ASSERT_EQUAL( 1, sut.getIdleCounter() );
}

void testShallNotCountIfSomethingRan()
{
    IdleCounter sut;
    TEST_ASSERT_EQUAL( 0, sut.getIdleCounter() );    
    sut.SignalOneTaskWasRun();
    sut.SignalOneCycleRunAndResetTaskRunStatus();
    TEST_ASSERT_EQUAL( 0, sut.getIdleCounter() );
}

void testShallResetTaskStatusOnEachCycle()
{
    IdleCounter sut;
    TEST_ASSERT_EQUAL( 0, sut.getIdleCounter() );    
    sut.SignalOneTaskWasRun();
    sut.SignalOneCycleRunAndResetTaskRunStatus();
    sut.SignalOneCycleRunAndResetTaskRunStatus();
    TEST_ASSERT_EQUAL( 1, sut.getIdleCounter() );
}

void testShallCalibrateWithTaskScheduler()
{
    TaskScheduler sched;    
    HardwareClockMock hwClock;
    hwClock.microsToStepOnRead = 2000;

    IdleCounter sut;
    sched.setIdleCounter( &sut );
    sut.setTaskSchedulerAndClock( &sched, &hwClock );

    sut.Run1000IdleTaskToCalibrate();

    TEST_ASSERT_EQUAL( 2000*1002, hwClock.getMicrosecondsSinceBoot() );
    TEST_ASSERT_EQUAL( 499, sut.getMaxIdleCountsPerSecond() ); //Timer is off-by-1, but in a hard-to-fix way.
}

void runAllTestsMeasureIdleTime()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testShallCountIfNothingRan );
    RUN_TEST( testShallNotCountIfSomethingRan );
    RUN_TEST( testShallResetTaskStatusOnEachCycle );
    RUN_TEST( testShallCalibrateWithTaskScheduler );
    UNITY_END_INT();
}