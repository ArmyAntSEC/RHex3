#include <VolatileRotationPositionWithLaps.h>
#include <unity.h>
#include <LevelLogger.h>

void testVolatileSetAndReadPostion()
{
    int clickPosition = 234;
    int laps = 3;
    VolatileRotationPositionWithLaps pos( clickPosition, laps );

    TEST_ASSERT_EQUAL( clickPosition, pos.getClickPosition() );
    TEST_ASSERT_EQUAL( laps, pos.getLaps() );

}

void testVolatileConvertClicksToPrecisePosition()
{

    VolatileRotationPositionWithLaps pos( 3156, 0 );        
    
    TEST_ASSERT_EQUAL ( 3156 - 1796, pos.getClickPosition()  );
    TEST_ASSERT_EQUAL ( 1, pos.getLaps()  );
    TEST_ASSERT_EQUAL ( 1e6-962666, pos.getRemainderMicroClicks() );
}

void testVolatileConvertClicksToPrecisePosition2Laps()
{

    VolatileRotationPositionWithLaps pos( 4200, 0 );        
    //4200 - 2*1795.962666 = 608.074668
    TEST_ASSERT_EQUAL ( 608, pos.getClickPosition()  );
    TEST_ASSERT_EQUAL ( 2, pos.getLaps()  );
    TEST_ASSERT_EQUAL( 74668, pos.getRemainderMicroClicks() );
}

void runAllTestsVolatileRotationPositionWithLap()
{        
    RUN_TEST( testVolatileSetAndReadPostion );    
    RUN_TEST( testVolatileConvertClicksToPrecisePosition );
    RUN_TEST( testVolatileConvertClicksToPrecisePosition2Laps );
};