#include <unityEx.h>
#include <HardwareClock.h>
#include <HardwareInterrupts.h>
#define private public

#include <SpeedComputer.h>
HardwareClockMock hwClock;

void testShouldComputeZeroSpeedIfNothingHappens()
{
    HardwareInterruptsMock hwInterrupts;
    SpeedComputer sut( &hwClock, &hwInterrupts );
    TEST_ASSERT_EQUAL( 0, sut.getSpeedCPS() );
}

void testShouldComputeSpeedAfterClicks()
{
    HardwareInterruptsMock hwInterrupts;
    SpeedComputer sut( &hwClock, &hwInterrupts );
    hwClock.resetMicrosecondsSinceBoot();
    hwClock.stepMicrosecondsSinceBoot( 1000 );
    sut.signalStepForwardISR();

    TEST_ASSERT_EQUAL( 1000, sut.getSpeedCPS() );
}

void testSpeedComputerShouldRestoreInterruptFlags()
{    
    HardwareInterruptsMock hwInterrupts;
    SpeedComputer sut( &hwClock, &hwInterrupts );    
    
    sut.getSpeedCPS();

    TEST_ASSERT_EQUAL( 1, hwInterrupts.interruptsEnabledCount );
    TEST_ASSERT_EQUAL( 1, hwInterrupts.interruptsDisabledCount );         
}

void runAllTestsSpeedComputer()
{    
    UNITY_BEGIN_INT();        
    RUN_TEST( testShouldComputeZeroSpeedIfNothingHappens );    
    RUN_TEST( testShouldComputeSpeedAfterClicks );
    RUN_TEST ( testSpeedComputerShouldRestoreInterruptFlags );
    UNITY_END_INT();
}