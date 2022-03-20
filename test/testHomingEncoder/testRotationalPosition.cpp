#include <unityEx.h>

#define private public
#include <LinearPositionalEncoder.h>
#include <RotationalPosition.h>
#include <HardwareClock.h>
#include <SerialStream.h>

struct LinearPositionProviderMock: public LinearPositionProvider
{
    int32_t linearPos = 0;

    virtual int32_t getLinearPosition()
    {
        return linearPos;
    }

    virtual bool isHomed()
    {
        TEST_FAIL_MESSAGE( "Not meant to be called." );
        return false;
    }
    
    virtual void unHome()
    {
        TEST_FAIL_MESSAGE( "Not meant to be called." );
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
    RotationalPosition sut ( 2000 );

    TEST_ASSERT_EQUAL( 1, sut.getLaps() );
    TEST_ASSERT_EQUAL( 205, sut.getClicks() );  //Algorithm rounds up.
}

void testShouldHandleLargeWrapAround()
{    
    RotationalPosition sut ( 40000000 );

    TEST_ASSERT_EQUAL( 22272, sut.getLaps() );
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

    RotationalPositionEncoder sut(&linPos);    

    TEST_ASSERT_EQUAL( 1234, sut.getLinearPosition() );
}

void testShouldBeAbleToJumpToTheLapBeforeAnother()
{ 
    RotationalPosition pos( 1000 + 2*673486L/375 ); //Two laps + 1000 clicks
    
    pos.moveToLapBeforeRounded( 2000 );
    TEST_ASSERT_EQUAL( 3, pos.getLaps() );
    TEST_ASSERT_EQUAL( 1000, pos.getClicks() );
}

void testShouldBeAbleToJumpToTheLapBeforeAnotherReverse()
{ 
    RotationalPosition pos( 1000 + 2*673486L/375 ); //Two laps + 1000 clicks
    
    pos.moveToLapBeforeRounded( 500 );    
    TEST_ASSERT_EQUAL( 2, pos.getLaps() );
    TEST_ASSERT_EQUAL( 1000, pos.getClicks() );
}

void shouldBeConstructableFromLapsAndClicks()
{
    RotationalPosition pos( 2, 1000 ); //Two laps + 1000 clicks
    
    TEST_ASSERT_EQUAL( 2, pos.getLaps() );
    TEST_ASSERT_EQUAL( 1000, pos.getClicks() );
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
    RUN_TEST( testShouldBeAbleToJumpToTheLapBeforeAnother );
    RUN_TEST( testShouldBeAbleToJumpToTheLapBeforeAnotherReverse );
    RUN_TEST( shouldBeConstructableFromLapsAndClicks );
    UNITY_END_INT();
}