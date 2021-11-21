#include <unity.h>

#include "MockEEPROMBackedArray.h"

#define private public
#include <SpeedToPowerConverter.h>

void testCreate()
{
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );
    
    TEST_ASSERT_EQUAL( &array, converter.data );
}

void testGetPowerForFreeSpeed()
{
    MockEEPROMBackedArray<2,8> array;
    Interpolator interpolator;
    SpeedToPowerConverter converter( &array );

    int result = converter.GetPowerForFreeSpeed( 2 );

    TEST_ASSERT_EQUAL ( 4, result );
}


void testFreeSpeedForPower()
{
    MockEEPROMBackedArray<2,8> array;
    Interpolator interpolator;
    SpeedToPowerConverter converter( &array );

    int result = converter.GetFreeSpeedForPower( 4 );

    TEST_ASSERT_EQUAL ( 2, result );
}

void processSpeedToPowerConverter()
{
    RUN_TEST( testCreate );   
    RUN_TEST( testGetPowerForFreeSpeed ); 
    RUN_TEST( testFreeSpeedForPower );     
}