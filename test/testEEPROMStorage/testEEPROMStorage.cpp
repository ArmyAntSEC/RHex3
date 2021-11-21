#include <unity.h>

#define private public
#include <EEPROMStorage.h>

void setUp(void) {    
    HardwareInterface::resetValues();    
}

void testConvertIndexToFirstAddress()
{
    EEPROMStorage eeprom;

    int address = eeprom.convertIndexToFirstAddress( 3 );

    TEST_ASSERT_EQUAL( 6, address );
}

void testConvertIndexToSecondAddress()
{
    EEPROMStorage eeprom;

    int address = eeprom.convertIndexToSecondAddress( 3 );

    TEST_ASSERT_EQUAL( 7, address );
}

void testWriteToIndex()
{
    EEPROMStorage eeprom;

    eeprom.writeIntToIndex( 5, 1234 );

    TEST_ASSERT_EQUAL( 210, HardwareInterface::EEPROMData[10] );
    TEST_ASSERT_EQUAL( 4, HardwareInterface::EEPROMData[11] );
}

void processEEPROMStorage()
{
    RUN_TEST( testConvertIndexToFirstAddress );
    RUN_TEST( testConvertIndexToSecondAddress );
    RUN_TEST( testWriteToIndex );
}