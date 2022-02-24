#pragma once
#include <unity.h>

#define private public
#include <LinearPositionalEncoder.h>

void testShouldStepForward()
{
    HardwareInterrupts hwInterrupts;    
    LinearPositionEncoder sut( &hwInterrupts );
    TEST_ASSERT_EQUAL( 0, sut.getLinearPosition() );

    sut.signalStepForwardISR();

    TEST_ASSERT_EQUAL( 1, sut.getLinearPosition() );
}

void testShouldHomeIfNotHomed()
{
    HardwareInterrupts hwInterrupts;    
    LinearPositionEncoder sut( &hwInterrupts );

    sut.linearPosition = 100;

    sut.signalHomingISR();

    TEST_ASSERT_EQUAL( 0, sut.getLinearPosition() );
}

void testShouldNotHomeIfHomed()
{
    HardwareInterrupts hwInterrupts;    
    LinearPositionEncoder sut( &hwInterrupts );

    sut.linearPosition = 100;
    sut.isHomed = true;

    sut.signalHomingISR();

    TEST_ASSERT_EQUAL( 100, sut.getLinearPosition() );
}

void runAllTestsLinearPositionEncoder()
{
    RUN_TEST( testShouldStepForward );   
    RUN_TEST( testShouldHomeIfNotHomed ); 
    RUN_TEST( testShouldNotHomeIfHomed ); 
}