#include <unity.h>
#include "MotorSpeedRegulator.h"

void testGetPowerforFreeSpeedOnStep (void) 
{
    TEST_ASSERT_EQUAL( 20, MotorSpeedRegulator::GetPowerForFreeSpeed(780) );
    TEST_ASSERT_EQUAL( 24, MotorSpeedRegulator::GetPowerForFreeSpeed(1363) );
    TEST_ASSERT_EQUAL( 32, MotorSpeedRegulator::GetPowerForFreeSpeed(2145) );
    TEST_ASSERT_EQUAL( 48, MotorSpeedRegulator::GetPowerForFreeSpeed(3472) );
    TEST_ASSERT_EQUAL( 64, MotorSpeedRegulator::GetPowerForFreeSpeed(4507) );
    TEST_ASSERT_EQUAL( 128, MotorSpeedRegulator::GetPowerForFreeSpeed(6509) );
    TEST_ASSERT_EQUAL( 255, MotorSpeedRegulator::GetPowerForFreeSpeed(7735) );        
}

void testGetPowerforFreeSpeedExtrapolate (void) 
{
    TEST_ASSERT_EQUAL( 20, MotorSpeedRegulator::GetPowerForFreeSpeed(600) );
    TEST_ASSERT_EQUAL( 255, MotorSpeedRegulator::GetPowerForFreeSpeed(8000) );
}

void testGetPowerforFreeSpeedInterpolate (void) 
{    
    TEST_ASSERT_INT_WITHIN( 2, 30, MotorSpeedRegulator::GetPowerForFreeSpeed(2000) );
    TEST_ASSERT_INT_WITHIN( 2, 111, MotorSpeedRegulator::GetPowerForFreeSpeed(6000) );    
}