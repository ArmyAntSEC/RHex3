#include <unityEx.h>

#define private public
#include <LinearPositionalEncoder.h>
#include <RotationalPosition.h>
#include <HardwareClock.h>
#include <SerialStream.h>

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
    RotationalPosition sut ( 0 );

    TEST_ASSERT_EQUAL( 0, sut.getLaps() );
    TEST_ASSERT_EQUAL( 0, sut.getClicks() );   
}

void testShouldGiveSameForSmallValues()
{    
    RotationalPosition sut ( 400 );

    TEST_ASSERT_EQUAL( 0, sut.getLaps() );
    TEST_ASSERT_EQUAL( 400, sut.getClicks() );   
}

void testShouldHandleWrapAround()
{    
    RotationalPosition sut ( 4000 );

    TEST_ASSERT_EQUAL( 1, sut.getLaps() );
    TEST_ASSERT_EQUAL( 409, sut.getClicks() );  //Algorithm rounds up.
}

void testShouldHandleLargeWrapAround()
{    
    RotationalPosition sut ( 40000000 );

    TEST_ASSERT_EQUAL( 11136, sut.getLaps() );
    TEST_ASSERT_EQUAL( 320, sut.getClicks() ); //Algorithm rounds up.      
}

void testSupportsEquality()
{
    RotationalPosition sut1 ( 1 );
    RotationalPosition sut2 ( 2 );
    RotationalPosition sut3 ( 2 );

    TEST_ASSERT( sut1 != sut2 );
    TEST_ASSERT( sut2 == sut3 );
}

void testRotationalPositionEncoder()
{
    LinearPositionProviderMock linPos;
    linPos.linearPos = 1234;

    RotationalPositionEncoder sut( &linPos );

    TEST_ASSERT_EQUAL( 1234, sut.getLinearPosition() );
}

void testPerformanceGetClicks()
{    
#ifdef ARDUINO
    RotationalPosition sut ( 40000000 );
    HardwareClock hwClock;

    unsigned long startTimeMicros = hwClock.getMicrosecondsSinceBoot();
    for ( int i = 0; i < 100; i++ )
    {
        volatile long value = sut.getClicks();
    }
    unsigned long endTimeMicros = hwClock.getMicrosecondsSinceBoot();
    TEST_ASSERT_LESS_THAN_INT32( 1000, endTimeMicros - startTimeMicros );

#else
    TEST_IGNORE_MESSAGE( "No performance measurements on Native" );
#endif
}

void runAllTestRotationalPosition()
{    
    UNITY_BEGIN_INT();
    RUN_TEST( testShouldGiveZeroForZero );
    RUN_TEST( testShouldGiveSameForSmallValues );
    RUN_TEST( testShouldHandleWrapAround );
    RUN_TEST( testShouldHandleLargeWrapAround );
    RUN_TEST( testSupportsEquality );
    RUN_TEST( testRotationalPositionEncoder );
    RUN_TEST( testPerformanceGetClicks );
    UNITY_END_INT();
}