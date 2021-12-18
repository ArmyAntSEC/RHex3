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

    TEST_ASSERT_EQUAL( 210, HardwareInterface::ReadEEPROM(10) );
    TEST_ASSERT_EQUAL( 4, HardwareInterface::ReadEEPROM(11) );
}

void testReadFromIndex()
{
    EEPROMStorage eeprom;
    eeprom.writeIntToIndex( 5, 1234 );

    int rValue = eeprom.readIntFromIndex( 5 );

    TEST_ASSERT_EQUAL( 1234, rValue );
}

void testWriteArrayToIndex()
{
    EEPROMStorage eeprom;    
    int values[] = {1234, 7658};
    
    eeprom.writeIntArrayToAddress( 5, values, 2 );

    TEST_ASSERT_EQUAL( 210, HardwareInterface::ReadEEPROM(10) );    
    TEST_ASSERT_EQUAL( 4, HardwareInterface::ReadEEPROM(11) );    
    TEST_ASSERT_EQUAL( 234, HardwareInterface::ReadEEPROM(12) );    
    TEST_ASSERT_EQUAL( 29, HardwareInterface::ReadEEPROM(13) );    
}

void testReadArrayFromIndex()
{
    EEPROMStorage eeprom;    
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