#include <unity.h>
#include "SpeedToPowerConverter.h"
#include "EEPROMStorage.h"

void testGetPowerforFreeSpeedOnStep (void) 
{
    TEST_IGNORE();
    SpeedToPowerConverterTest converter;
    //Power 0 == Speed 0, or else the logic is broken. So make this test pass.
    TEST_ASSERT_EQUAL( 0, converter.GetPowerForFreeSpeed(0) );    
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
    TEST_IGNORE();
    SpeedToPowerConverterTest converter;
    //The standard dataset foes to 0 so extrapolation downwards isn't possible.    
    TEST_ASSERT_EQUAL( 255, converter.GetPowerForFreeSpeed(8000) );
}

void testGetPowerforFreeSpeedInterpolate (void) 
{    
    TEST_IGNORE();
    SpeedToPowerConverterTest converter;
    TEST_ASSERT_INT_WITHIN( 1, 30, converter.GetPowerForFreeSpeed(2000) );
    TEST_ASSERT_INT_WITHIN( 1, 111, converter.GetPowerForFreeSpeed(6000) );    
}

void testGetFreeSpeedForPowerInterpolate (void) 
{    
    TEST_IGNORE();
    SpeedToPowerConverterTest converter;
    TEST_ASSERT_INT_WITHIN( 100, 2000, converter.GetFreeSpeedForPower(30) );
    TEST_ASSERT_INT_WITHIN( 100, 6000, converter.GetFreeSpeedForPower(111) );    
}


void testInitSpeedToPowerConverterFromEEPROM()
{
    #ifndef ARDUINO
    TEST_IGNORE();
    #endif

    SpeedToPowerConverterTest converter;
    TEST_ASSERT_EQUAL( 0, converter.speedVsPower[0][0] );
    
    EEPROMStorage eeprom;
    eeprom.writeIntToIndex( 96, 56 ); //Overwrite the first value of the default storage location
    converter.initFromEEPROM();

    TEST_ASSERT_EQUAL( 56, converter.speedVsPower[0][0] );
}

void testSaveSpeedToPowerConverterToEEPROM()
{
    #ifndef ARDUINO
    TEST_IGNORE();
    #endif
    
    const int converterEEPROMStartIndex = 96;
    EEPROMStorage eeprom;
    eeprom.writeIntToIndex( converterEEPROMStartIndex, 56 );  //Overwrite the first value of the default storage location

    SpeedToPowerConverterTest converter;
    converter.saveToEEPROM();
    
    int valueRead = eeprom.readIntFromIndex( converterEEPROMStartIndex );    
    
    TEST_ASSERT_EQUAL( 0, valueRead );
}

void runAllTestsSpeedVsPowerConverter()
{
    //Speed vs Power conversions    
    RUN_TEST(testGetPowerforFreeSpeedExtrapolate);    
    RUN_TEST(testGetPowerforFreeSpeedOnStep);    
    RUN_TEST(testGetPowerforFreeSpeedInterpolate);    
    RUN_TEST(testGetFreeSpeedForPowerInterpolate);
    
    //Interact betwen converter and EEPROM.    
    RUN_TEST(testInitSpeedToPowerConverterFromEEPROM);    
    RUN_TEST(testSaveSpeedToPowerConverterToEEPROM);    
}