#include <unity.h>

#include "MockEEPROMBackedArray.h"
#include "MockInterpolator.h"

#define private public
#include <SpeedToPowerConverter.h>

void testCreate()
{
    MockEEPROMBackedArray<2,8> array;
    MockInterpolator interpolator;
    SpeedToPowerConverter converter( &array, &interpolator );
    
    TEST_ASSERT_EQUAL( &array, converter.data );
}

void testGetPowerForFreeSpeed()
{
    MockEEPROMBackedArray<2,8> array;
    MockInterpolator interpolator;
    SpeedToPowerConverter converter( &array, &interpolator );

    int result = converter.GetPowerForFreeSpeed( 2 );

    TEST_ASSERT_EQUAL ( 4, result );
}


void processSpeedToPowerConverter()
{
    RUN_TEST( testCreate );   
    RUN_TEST( testGetPowerForFreeSpeed ); 
}