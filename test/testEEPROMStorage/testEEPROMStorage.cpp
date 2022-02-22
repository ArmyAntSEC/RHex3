#include <unity.h>

#define private public
#include <EEPROMStorage.h>

HardwareEEPROM HWEeprom;

void setUp(void) {    
    HWEeprom.resetValues();    
}

void testConvertIndexToFirstAddress()
{
    EEPROMStorage eeprom( &HWEeprom );

    int address = eeprom.convertIndexToFirstAddress( 3 );

    TEST_ASSERT_EQUAL( 6, address );
}

void testConvertIndexToSecondAddress()
{
    EEPROMStorage eeprom( &HWEeprom );

    int address = eeprom.convertIndexToSecondAddress( 3 );

    TEST_ASSERT_EQUAL( 7, address );
}

void testWriteToIndex()
{
    EEPROMStorage eeprom( &HWEeprom );

    eeprom.writeIntToIndex( 5, 1234 );

    TEST_ASSERT_EQUAL( 210, HWEeprom.EEPROMData[10] );
    TEST_ASSERT_EQUAL( 4, HWEeprom.EEPROMData[11] );
}

void testReadFromIndex()
{
    EEPROMStorage eeprom( &HWEeprom );
    eeprom.writeIntToIndex( 5, 1234 );

    int rValue = eeprom.readIntFromIndex( 5 );

    TEST_ASSERT_EQUAL( 1234, rValue );
}

void testWriteArrayToIndex()
{
    EEPROMStorage eeprom( &HWEeprom );    
    int values[] = {1234, 7658};
    
    eeprom.writeIntArrayToAddress( 5, values, 2 );

    TEST_ASSERT_EQUAL( 210, HWEeprom.EEPROMData[10] );    
    TEST_ASSERT_EQUAL( 4, HWEeprom.EEPROMData[11] );    
    TEST_ASSERT_EQUAL( 234, HWEeprom.EEPROMData[12] );    
    TEST_ASSERT_EQUAL( 29, HWEeprom.EEPROMData[13] );    
}

void testReadArrayFromIndex()
{
    EEPROMStorage eeprom( &HWEeprom );    
    int values[] = {1234, 7658};    
    eeprom.writeIntArrayToAddress( 5, values, 2 );

    int rValues[2];
    eeprom.readIntArrayFromAddress( 5, rValues, 2 );

    TEST_ASSERT_EQUAL( 1234, rValues[0] );    
    TEST_ASSERT_EQUAL( 7658, rValues[1] );        
}

void processEEPROMStorage()
{
    RUN_TEST( testConvertIndexToFirstAddress );
    RUN_TEST( testConvertIndexToSecondAddress );
    RUN_TEST( testWriteToIndex );
    RUN_TEST( testReadFromIndex );
    RUN_TEST( testWriteArrayToIndex );
    RUN_TEST( testReadArrayFromIndex );
}