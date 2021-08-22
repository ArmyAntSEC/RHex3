#include <unity.h>
#include "SpeedToPowerConverter.h"
#include "EEPROMStorage.h"

void testGetPowerforFreeSpeedOnStep (void) 
{
    SpeedToPowerConverter converter;
    TEST_ASSERT_EQUAL( 20, converter.GetPowerForFreeSpeed(780) );
    TEST_ASSERT_EQUAL( 24, converter.GetPowerForFreeSpeed(1363) );
    TEST_ASSERT_EQUAL( 32, converter.GetPowerForFreeSpeed(2145) );
    TEST_ASSERT_EQUAL( 48, converter.GetPowerForFreeSpeed(3472) );
    TEST_ASSERT_EQUAL( 64, converter.GetPowerForFreeSpeed(4507) );
    TEST_ASSERT_EQUAL( 128, converter.GetPowerForFreeSpeed(6509) );
    TEST_ASSERT_EQUAL( 255, converter.GetPowerForFreeSpeed(7735) );        
}

void testGetPowerforFreeSpeedExtrapolate (void) 
{
    SpeedToPowerConverter converter;
    TEST_ASSERT_EQUAL( 20, converter.GetPowerForFreeSpeed(600) );
    TEST_ASSERT_EQUAL( 255, converter.GetPowerForFreeSpeed(8000) );
}

void testGetPowerforFreeSpeedInterpolate (void) 
{    
    SpeedToPowerConverter converter;
    TEST_ASSERT_INT_WITHIN( 1, 30, converter.GetPowerForFreeSpeed(2000) );
    TEST_ASSERT_INT_WITHIN( 1, 111, converter.GetPowerForFreeSpeed(6000) );    
}

void testGetFreeSpeedForPowerInterpolate (void) 
{    
    SpeedToPowerConverter converter;
    TEST_ASSERT_INT_WITHIN( 100, 2000, converter.GetFreeSpeedForPower(30) );
    TEST_ASSERT_INT_WITHIN( 100, 6000, converter.GetFreeSpeedForPower(111) );    
}


void testInitSpeedToPowerConverterFromEEPROM()
{

    SpeedToPowerConverter converter;
    TEST_ASSERT_EQUAL( 20, converter.speedVsPower[0][0] );
    
    EEPROMStorage eeprom;
    eeprom.writeIntToIndex( 96, 56 ); //Overwrite the first value of the default storage location
    converter.initFromEEPROM();

    TEST_ASSERT_EQUAL( 56, converter.speedVsPower[0][0] );
}

void testSaveSpeedToPowerConverterToEEPROM()
{
    const int converterEEPROMStartIndex = 96;
    EEPROMStorage eeprom;
    eeprom.writeIntToIndex( converterEEPROMStartIndex, 56 );  //Overwrite the first value of the default storage location

    SpeedToPowerConverter converter;
    converter.saveToEEPROM();
    
    int valueRead = eeprom.readIntFromIndex( converterEEPROMStartIndex );
    
    
    TEST_ASSERT_EQUAL( 20, valueRead );
}