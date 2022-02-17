#pragma once
#include <unity.h>

#define private public
#include <LinearPositionalEncoder.h>
#include <RotationalPosition.h>

struct LinearPositionProviderMock: public LinearPositionProvider
{
    long linearPos = 0;

    virtual long getLinearPosition()
    {
        return linearPos;
    }
};

void testShouldGiveZeroForZero()
{
    LinearPositionProviderMock linPos;    
    RotationalPosition sut ( &linPos );

    TEST_ASSERT_EQUAL( 0, sut.getLaps() );
    TEST_ASSERT_EQUAL( 0, sut.getClicks() );   
    TEST_ASSERT_EQUAL( 0, sut.getRemainder() );
}

void runAllTestRotationalPosition()
{
    UNITY_BEGIN();
    RUN_TEST( testShouldGiveZeroForZero );
    UNITY_END();
}