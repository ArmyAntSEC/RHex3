#pragma once
#include <unity.h>
#include <HardwareClock.h>
#include <HardwareInterrupts.h>
#define private public

#include <SpeedComputer.h>
HardwareClock HWClock;
HardwareInterrupts HWInterrupts;

void testShouldComputeZeroSpeedIfNothingHappens()
{
    SpeedComputer sut( &HWClock, &HWInterrupts );
    TEST_ASSERT_EQUAL( 0, sut.getSpeedCPS() );
}

void testShouldComputeSpeedAfterClicks()
{
    SpeedComputer sut( &HWClock, &HWInterrupts );
    HWClock.resetMicrosecondsSinceBoot();
    HWClock.stepMicrosecondsSinceBoot( 1000 );
    sut.signalStepForwardISR();

    TEST_ASSERT_EQUAL( 1000, sut.getSpeedCPS() );
}

void runAllTestsSpeedComputer()
{
    UNITY_BEGIN();
    RUN_TEST( testShouldComputeZeroSpeedIfNothingHappens );
    RUN_TEST( testShouldComputeSpeedAfterClicks );
    UNITY_END();
}