#include <VolatileRotationPositionWithLaps.h>
#include <unity.h>

void testVolatileSetAndReadPostion()
{
    int clickPosition = 234;
    int laps = 3;
    VolatileRotationPositionWithLaps pos( clickPosition, laps );

    TEST_ASSERT_EQUAL( clickPosition, pos.getClickPositionISR() );
    TEST_ASSERT_EQUAL( laps, pos.getLapsISR() );

}

void testVolatileConvertClicksToPrecisePosition()
{

    VolatileRotationPositionWithLaps pos( 3156, 0 );        
    
    TEST_ASSERT_EQUAL ( 3156 - 1796, pos.getClickPositionISR()  );
    TEST_ASSERT_EQUAL ( 1, pos.getLapsISR()  );
    TEST_ASSERT_EQUAL ( 1e6-962666, pos.getRemainderMicroClicksISR() );
}

void testVolatileConvertClicksToPrecisePosition2Laps()
{

    VolatileRotationPositionWithLaps pos( 4200, 0 );        
    //4200 - 2*1795.962666 = 608.074668
    TEST_ASSERT_EQUAL ( 608, pos.getClickPositionISR()  );
    TEST_ASSERT_EQUAL ( 2, pos.getLapsISR()  );
    TEST_ASSERT_EQUAL( 74668, pos.getRemainderMicroClicksISR() );
}

void testVolatileConvertToNonVolatileCopy()
{

    VolatileRotationPositionWithLaps volatilePos( 1234, 1, 54321 );
    RotationPositionWithLaps nonVolatilePos = volatilePos.getNonVolatileCopy();

    TEST_ASSERT_EQUAL ( 1234, nonVolatilePos.getClickPosition()  );
    TEST_ASSERT_EQUAL ( 1, nonVolatilePos.getLaps()  );
    TEST_ASSERT_DOUBLE_WITHIN( 1e-4, 0.054321, nonVolatilePos.getRemainder() );
    TEST_ASSERT_INT_WITHIN( 100, 54321, nonVolatilePos.getRemainderMicroClicks() );
}

void runAllTestsVolatileRotationPositionWithLap()
{        
    RUN_TEST( testVolatileSetAndReadPostion );    
    RUN_TEST( testVolatileConvertClicksToPrecisePosition );
    RUN_TEST( testVolatileConvertClicksToPrecisePosition2Laps );
    RUN_TEST( testVolatileConvertToNonVolatileCopy );
};