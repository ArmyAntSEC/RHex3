#include <unityEx.h>
#define private public
#include <TaskScheduler.h>
#include <HardwareClock.h>
#include <IdleCounter.h>

void testShallCountIfNothingRan()
{
    HardwareClockMock hwClock;
    TaskScheduler scheduler(&hwClock);
    IdleCounter* sut = scheduler.getIdleCounterObject();

    TEST_ASSERT_EQUAL( 0, sut->getIdleCounter() );    
    sut->SignalOneCycleRunAndResetTaskRunStatus();
    TEST_ASSERT_EQUAL( 1, sut->getIdleCounter() );
}

void testShallNotCountIfSomethingRan()
{
    HardwareClockMock hwClock;
    TaskScheduler scheduler(&hwClock);
    IdleCounter* sut = scheduler.getIdleCounterObject();

    TEST_ASSERT_EQUAL( 0, sut->getIdleCounter() );    
    sut->SignalOneTaskWasRun();
    sut->SignalOneCycleRunAndResetTaskRunStatus();
    TEST_ASSERT_EQUAL( 0, sut->getIdleCounter() );
}

void testShallResetTaskStatusOnEachCycle()
{
    HardwareClockMock hwClock;
    TaskScheduler scheduler(&hwClock);
    IdleCounter* sut = scheduler.getIdleCounterObject();

    TEST_ASSERT_EQUAL( 0, sut->getIdleCounter() );    
    sut->SignalOneTaskWasRun();
    sut->SignalOneCycleRunAndResetTaskRunStatus();
    sut->SignalOneCycleRunAndResetTaskRunStatus();
    TEST_ASSERT_EQUAL( 1, sut->getIdleCounter() );
}

void testShallBeAbleToCalibrateWithTaskScheduler()
{
    HardwareClockMock hwClock;
    TaskScheduler scheduler(&hwClock);
    IdleCounter* sut = scheduler.getIdleCounterObject();

    hwClock.microsToStepOnRead = 2000;

    sut->setTaskSchedulerAndClock( &scheduler, &hwClock );    
    sut->Run1000IdleTaskToCalibrateAndGetMaxIdleCountsPerSecond();

    TEST_ASSERT_EQUAL( 499, sut->maxIdleCountsPerSecond ); //Timer is off-by-1, but in a hard-to-fix way.
    TEST_ASSERT_EQUAL( 2000*1002, hwClock.getMicrosecondsSinceBoot() );
}

void testShallBeAbleToResetCounterAndComputeTasksPerSecond()
{
    HardwareClockMock hwClock;
    TaskScheduler sched(&hwClock);

    IdleCounter* sut = sched.getIdleCounterObject();    
    sut->setTaskSchedulerAndClock( &sched, &hwClock );
        
    sut->SignalOneCycleRunAndResetTaskRunStatus();    
    
    hwClock.microsSinceStart = 100;

    TEST_ASSERT_EQUAL( 10000, sut->getIdleCountsPerSecondAndResetCounter() );

    TEST_ASSERT_EQUAL( 0, sut->getIdleCounter() );    
}

void testShallComputeCPULoad()
{
    HardwareClockMock hwClock;
    TaskScheduler scheduler(&hwClock);
    IdleCounter* sut = scheduler.getIdleCounterObject();

    sut->maxIdleCountsPerSecond = 20;    
    sut->SignalOneCycleRunAndResetTaskRunStatus();
    TEST_ASSERT_EQUAL( 5, sut->getCPUFactorPercent() );    
}

void runAllTestsMeasureIdleTime()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testShallCountIfNothingRan );
    RUN_TEST( testShallNotCountIfSomethingRan );
    RUN_TEST( testShallResetTaskStatusOnEachCycle );
    RUN_TEST( testShallBeAbleToCalibrateWithTaskScheduler );
    RUN_TEST( testShallBeAbleToResetCounterAndComputeTasksPerSecond );
    RUN_TEST( testShallComputeCPULoad );
    UNITY_END_INT();
}