#include <RotationPositionWithLap.h>
#include <unity.h>
#include <LevelLogger.h>

void testSetAndReadPostion()
{
    int clickPosition = 234;
    int laps = 3;
    RotationPositionWithLap pos( clickPosition, laps );

    TEST_ASSERT_EQUAL( clickPosition, pos.getClickPosition() );
    TEST_ASSERT_EQUAL( laps, pos.getLaps() );

}

void testGetClicksPerRotation()
{

    RotationPositionWithLap pos( 0, 0 );    
    SQ15x16 clicksPerRotation = pos.getClicksPerRotation();
    
    TEST_ASSERT_DOUBLE_WITHIN ( 1e-5, 1795.9626665, (double)clicksPerRotation  );
}

void testConvertClicksToPrecisePosition()
{

    RotationPositionWithLap pos( 3156, 0 );        
    
    TEST_ASSERT_DOUBLE_WITHIN ( 1e-5, 3156 - 1796, (double)pos.getClickPosition()  );
    TEST_ASSERT_EQUAL ( 1, pos.getLaps()  );
    TEST_ASSERT_DOUBLE_WITHIN( 1e-4, 1-0.9626665, (double)pos.getRemainder() );
}

void testConvertClicksToPrecisePosition2Laps()
{

    RotationPositionWithLap pos( 4200, 0 );        
    //4200 - 2*1795.9626665 = 608.074667
    TEST_ASSERT_DOUBLE_WITHIN ( 1e-5, 608, (double)pos.getClickPosition()  );
    TEST_ASSERT_EQUAL ( 2, pos.getLaps()  );
    TEST_ASSERT_DOUBLE_WITHIN( 1e-4, 0.074667, (double)pos.getRemainder() );
}

void testIncrement()
{

    RotationPositionWithLap pos( 2000, 0 );        
    pos.incrementClicks( 2200 );        
    //4200 - 2*1795.9626665 = 608.074667
    TEST_ASSERT_DOUBLE_WITHIN ( 1e-5, 608, (double)pos.getClickPosition()  );
    TEST_ASSERT_EQUAL ( 2, pos.getLaps()  );
    TEST_ASSERT_DOUBLE_WITHIN( 1e-4, 0.074667, (double)pos.getRemainder() );
}

void testPositiveDifferenceSimpleOnSameLap()
{
    RotationPositionWithLap pos1( 2500, 0 );        
    RotationPositionWithLap pos2( 2000, 0 );            
    TEST_ASSERT_EQUAL( 500, pos1.getShortestPositiveDifferenceInt( &pos2 ) );
}

void testPositiveDifferenceSimpleTenLapDifference()
{    
    RotationPositionWithLap pos1( 2500, 10 );        
    RotationPositionWithLap pos2( 2000, 0 );            
    TEST_ASSERT_EQUAL( 500, 
        pos1.getShortestPositiveDifferenceInt( &pos2 ) );
}

void testPositiveDifferenceReverseOnSameLap()
{ 
    RotationPositionWithLap pos1( 2000, 0 );        
    RotationPositionWithLap pos2( 2500, 0 );   
    TEST_ASSERT_EQUAL( pos1.getClicksPerRotation().getInteger() - 500, pos1.getShortestPositiveDifferenceInt( &pos2 ) );
}

void runAllTestsRotationPositionWithLap()
{        
    RUN_TEST( testSetAndReadPostion );
    RUN_TEST( testGetClicksPerRotation );
    RUN_TEST( testConvertClicksToPrecisePosition );
    RUN_TEST( testConvertClicksToPrecisePosition2Laps );
    RUN_TEST( testIncrement );
    RUN_TEST( testPositiveDifferenceSimpleOnSameLap);    
    RUN_TEST( testPositiveDifferenceSimpleTenLapDifference );
    RUN_TEST( testPositiveDifferenceReverseOnSameLap );
}