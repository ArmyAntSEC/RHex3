#include <RotationPositionWithLaps.h>
#include <unity.h>

void testSetAndReadPostion()
{
    int clickPosition = 234;
    int laps = 3;
    RotationPositionWithLaps pos( clickPosition, laps );

    TEST_ASSERT_EQUAL( clickPosition, pos.getClickPosition() );
    TEST_ASSERT_EQUAL( laps, pos.getLaps() );
}

void testGetClicksPerRotation()
{

    RotationPositionWithLaps pos( 0, 0 );    
    SQ15x16 clicksPerRotation = pos.getClicksPerRotation();
    
    TEST_ASSERT_DOUBLE_WITHIN ( 1e-5, 1795.9626665, (double)clicksPerRotation  );
}

void testConvertClicksToPrecisePosition()
{

    RotationPositionWithLaps pos( 3156, 0 );        
    
    TEST_ASSERT_DOUBLE_WITHIN ( 1e-5, 3156 - 1796, (double)pos.getClickPosition()  );
    TEST_ASSERT_EQUAL ( 1, pos.getLaps()  );
    TEST_ASSERT_DOUBLE_WITHIN( 1e-4, 1-0.9626665, (double)pos.getRemainder() );
}

void testConvertClicksToPrecisePosition2Laps()
{

    RotationPositionWithLaps pos( 4200, 0 );        
    //4200 - 2*1795.9626665 = 608.074667
    TEST_ASSERT_DOUBLE_WITHIN ( 1e-5, 608, (double)pos.getClickPosition()  );
    TEST_ASSERT_EQUAL ( 2, pos.getLaps()  );
    TEST_ASSERT_DOUBLE_WITHIN( 1e-4, 0.074667, (double)pos.getRemainder() );
}

void testIncrement()
{

    RotationPositionWithLaps pos( 2000, 0 );        
    pos.incrementClicks( 2200 );        
    //4200 - 2*1795.9626665 = 608.074667
    TEST_ASSERT_DOUBLE_WITHIN ( 1e-5, 608, (double)pos.getClickPosition()  );
    TEST_ASSERT_EQUAL ( 2, pos.getLaps()  );
    TEST_ASSERT_DOUBLE_WITHIN( 1e-4, 0.074667, (double)pos.getRemainder() );
}

void testPositiveDifferenceSimpleOnSameLap()
{
    RotationPositionWithLaps pos1( 2500, 0 );        
    RotationPositionWithLaps pos2( 2000, 0 );            
    TEST_ASSERT_EQUAL( 500, pos1.getShortestPositiveDifferenceInt( &pos2 ) );
}

void testPositiveDifferenceSimpleTenLapDifference()
{    
    RotationPositionWithLaps pos1( 2500, 10 );        
    RotationPositionWithLaps pos2( 2000, 0 );            
    TEST_ASSERT_EQUAL( 500, 
        pos1.getShortestPositiveDifferenceInt( &pos2 ) );
}

void testPositiveDifferenceReverseOnSameLap()
{ 
    RotationPositionWithLaps pos1( 2000, 0 );        
    RotationPositionWithLaps pos2( 2500, 0 );   
    TEST_ASSERT_EQUAL( pos1.getClicksPerRotation().getInteger() - 500, pos1.getShortestPositiveDifferenceInt( &pos2 ) );
}

void testDifferenceSimpleOnSameLap()
{
    RotationPositionWithLaps pos1( 2500, 0 );        
    RotationPositionWithLaps pos2( 2000, 0 );            
    TEST_ASSERT_EQUAL( 500, pos1.getDifferenceInClicks( &pos2 ) );
}

void testDifferenceSimpleTenLapDifference()
{    
    RotationPositionWithLaps pos1( 2500, 10 );        
    RotationPositionWithLaps pos2( 2000, 0 );            
    TEST_ASSERT_EQUAL( 500 + (10 * pos1.getClicksPerRotation()).getInteger(), 
        pos1.getDifferenceInClicks( &pos2 ) );
}

void testDifferenceReverseOnSameLap()
{ 
    RotationPositionWithLaps pos1( 2000, 0 );        
    RotationPositionWithLaps pos2( 2500, 0 );   
    TEST_ASSERT_EQUAL( -500, pos1.getDifferenceInClicks( &pos2 ) );
}



void testMakeCopy()
{ 
    //Test the implicit copy constructor
    RotationPositionWithLaps pos1( 2000, 2 );
    RotationPositionWithLaps pos2( pos1 );   
    TEST_ASSERT_EQUAL( pos1.getClickPosition(), pos2.getClickPosition() );
    TEST_ASSERT_EQUAL( pos1.getLaps(), pos2.getLaps() );
    TEST_ASSERT_TRUE( pos1.getRemainder() == pos2.getRemainder() );
}

void testCompareEquals()
{ 
    //Test the implicit copy constructor
    RotationPositionWithLaps pos1( 2000, 2 );
    RotationPositionWithLaps pos2( pos1 );   
    TEST_ASSERT_TRUE( pos1 == pos2 );
}

void testCompareDiffInRemainder()
{
    RotationPositionWithLaps pos1( 1000, 2, 0 );
    RotationPositionWithLaps pos2( 1000, 2, 0.5 );   
    TEST_ASSERT_FALSE( pos1 == pos2 );
}

void testCompareDiffInLaps()
{
    RotationPositionWithLaps pos1( 1000, 2, 0 );
    RotationPositionWithLaps pos2( 1000, 3, 0 );   
    TEST_ASSERT_FALSE( pos1 == pos2 );
}

void testCompareDiffInClicks()
{
    RotationPositionWithLaps pos1( 1200, 2, 0 );
    RotationPositionWithLaps pos2( 1000, 2, 0 );   
    TEST_ASSERT_FALSE( pos1 == pos2 );
}

void testConvertToLongInt()
{
    RotationPositionWithLaps pos1( 1200, 10, 0.5 );    
    TEST_ASSERT_EQUAL_INT32( 1200 + (pos1.getClicksPerRotation()*10 + SQ15x16(0.5)).getInteger(), pos1.getSerialPosition() );
}

void testMoveForwardToSameLap()
{
    RotationPositionWithLaps pos1( 1200, 10, 0.5 );    
    pos1.moveForwardTo( 1500 );

    TEST_ASSERT_EQUAL( 1500, pos1.getClickPosition() );
    TEST_ASSERT_EQUAL( 10, pos1.getLaps() );
    TEST_ASSERT_TRUE( 0 == pos1.getRemainder() )
}

void testMoveForwardToOtherLap()
{
    RotationPositionWithLaps pos1( 1200, 10, 0.5 );    
    pos1.moveForwardTo( 5500 );

    TEST_ASSERT_EQUAL( 112, pos1.getClickPosition() );
    TEST_ASSERT_EQUAL( 13, pos1.getLaps() );
    TEST_ASSERT_DOUBLE_WITHIN( 1e-3, 0.1120, (double)pos1.getRemainder() );
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
    RUN_TEST( testMakeCopy );
    RUN_TEST( testCompareEquals );
    RUN_TEST( testCompareDiffInRemainder );
    RUN_TEST( testCompareDiffInLaps );
    RUN_TEST( testCompareDiffInClicks );
    RUN_TEST( testConvertToLongInt );
    RUN_TEST( testDifferenceSimpleOnSameLap );
    RUN_TEST( testDifferenceSimpleTenLapDifference );
    RUN_TEST( testDifferenceReverseOnSameLap );
    RUN_TEST( testMoveForwardToSameLap );
    RUN_TEST( testMoveForwardToOtherLap );
    
};