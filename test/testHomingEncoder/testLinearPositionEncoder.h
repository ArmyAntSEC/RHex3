#pragma once
#include <unity.h>

#define private public
#include <LinearPositionalEncoder.h>

void testShouldStepForward()
{
    LinearPositionEncoder sut;
    TEST_ASSERT_EQUAL( 0, sut.getLinearPosition() );

    sut.signalStepForwardISR();

    TEST_ASSERT_EQUAL( 1, sut.getLinearPosition() );
}

void testShouldHomeIfNotHomed()
{
    LinearPositionEncoder sut;
    sut.linearPosition = 100;

    sut.signalHomingISR();

    TEST_ASSERT_EQUAL( 0, sut.getLinearPosition() );
}

void testShouldNotHomeIfHomed()
{
    LinearPositionEncoder sut;
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