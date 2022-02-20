#pragma once
#include <unity.h>

#define private public
#include <LinearPositionalEncoder.h>

void testShouldStepForward()
{
    HardwareInterrupts interrupts;    
    LinearPositionEncoder sut( &interrupts );
    TEST_ASSERT_EQUAL( 0, sut.getLinearPosition() );

    sut.signalStepForwardISR();

    TEST_ASSERT_EQUAL( 1, sut.getLinearPosition() );
}

void testShouldHomeIfNotHomed()
{
    HardwareInterrupts interrupts;    
    LinearPositionEncoder sut( &interrupts );

    sut.linearPosition = 100;

    sut.signalHomingISR();

    TEST_ASSERT_EQUAL( 0, sut.getLinearPosition() );
}

void testShouldNotHomeIfHomed()
{
    HardwareInterrupts interrupts;    
    LinearPositionEncoder sut( &interrupts );

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