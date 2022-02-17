#pragma once
#include <unity.h>

#define private public

#include <SpeedComputer.h>

void testShouldComputeZeroSpeedIfNothingHappens()
{
    SpeedComputer sut;
    TEST_ASSERT_EQUAL( 0, sut.getSpeedCPS() );
}

void testShouldComputeSpeedAfterClicks()
{
    SpeedComputer sut;
    HardwareInterface::resetMicrosecondsSinceBoot();
    HardwareInterface::stepMicrosecondsSinceBoot( 1000 );
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