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
}

void testShouldGiveSameForSmallValues()
{
    LinearPositionProviderMock linPos;    
    linPos.linearPos = 400;
    RotationalPosition sut ( &linPos );

    TEST_ASSERT_EQUAL( 0, sut.getLaps() );
    TEST_ASSERT_EQUAL( 400, sut.getClicks() );   
}

void testShouldHandleWrapAround()
{
    LinearPositionProviderMock linPos;    
    linPos.linearPos = 4000;
    RotationalPosition sut ( &linPos );

    TEST_ASSERT_EQUAL( 1, sut.getLaps() );
    TEST_ASSERT_EQUAL( 408, sut.getClicks() );       
}

void testShouldHandleLargeWrapAround()
{
    LinearPositionProviderMock linPos;    
    linPos.linearPos = 40000000;
    RotationalPosition sut ( &linPos );

    TEST_ASSERT_EQUAL( 11136, sut.getLaps() );
    TEST_ASSERT_EQUAL( 319, sut.getClicks() );       
}

void runAllTestRotationalPosition()
{
    UNITY_BEGIN();
    RUN_TEST( testShouldGiveZeroForZero );
    RUN_TEST( testShouldGiveSameForSmallValues );
    RUN_TEST( testShouldHandleWrapAround );
    RUN_TEST( testShouldHandleLargeWrapAround );
    UNITY_END();
}