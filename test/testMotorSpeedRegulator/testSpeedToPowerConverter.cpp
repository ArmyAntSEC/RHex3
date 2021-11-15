#include <unity.h>

#include "MockEEPROMBackedArray.h"
#include "MockInterpolator.h"

#define private public
#include <SpeedToPowerConverter.h>

void testCreate()
{
    MockEEPROMBackedArray<8,2> array;
    MockInterpolator interpolator;
    SpeedToPowerConverter converter( &array, &interpolator );
    
    TEST_ASSERT_EQUAL( &array, converter.data );
}

void testGetPowerForFreeSpeed()
{
    MockEEPROMBackedArray<8,2> array;
    MockInterpolator interpolator;
    SpeedToPowerConverter converter( &array, &interpolator );

    int result = converter.GetPowerForFreeSpeed( 32 );

    TEST_ASSERT_EQUAL ( 42, result );
}


void processSpeedToPowerConverter()
{
    RUN_TEST( testCreate );   
    RUN_TEST( testGetPowerForFreeSpeed ); 
}