#include <unityEx.h>

#include "MockEEPROM.h"

#define private public
#include <EEPROMBackedArray.h>

void testConstructEEPROMStorage()
{
    MockEEPROMStorage eeprom;
    EEPROMBackedArray<10,2> array( &eeprom, 0 );
    TEST_ASSERT_EACH_EQUAL_INT( 0, array.values,10*2 );
}

void testLoadFromEEPROM()
{
    int16_t expected[2][2] = {{1, 2}, {3, 4}};
    MockEEPROMStorage eeprom;
    eeprom.rawData = (int*)expected;

    EEPROMBackedArray<2,2> array(&eeprom, 0);    
    array.loadFromEEPROM();
    
    TEST_ASSERT_EQUAL_INT_ARRAY( expected, array.values, 2*2 );
}

void testSetValue()
{
    MockEEPROMStorage eeprom;
    EEPROMBackedArray<2,2> array(&eeprom, 0);
    array.setValue(1,0,42);
    TEST_ASSERT_EQUAL_INT( 42, array.values[1][0] );
}

void testStoreToEEPROM()
{
    int16_t expected[2][2] = {{0, 0}, {0, 0}};
    MockEEPROMStorage eeprom;
    eeprom.rawData = (int*)expected;
    EEPROMBackedArray<2,2> array(&eeprom, 0);    
    array.setValue(1,0,42);
    
    array.storeToEEPROM();
    
    TEST_ASSERT_EQUAL_INT( 42, expected[1][0] );
}

void testGetSubArray()
{
    int16_t expected[2][2] = {{1, 2}, {3, 4}};
    MockEEPROMStorage eeprom;
    eeprom.rawData = (int*)expected;

    EEPROMBackedArray<2,2> array(&eeprom, 0);    
    array.loadFromEEPROM();

    int16_t const * subArray = array.getSubArray(1);

    TEST_ASSERT_EQUAL_INT_ARRAY( expected[1], subArray, 2 );           
}

void testGetArrayLength()
{
    EEPROMBackedArray<2,5> array(0, 0);    
    int16_t len = array.getArrayLength();
    
    TEST_ASSERT_EQUAL_INT( 5, len );
}


void processEEPROMBackedArrayStorage()
{    
    UNITY_BEGIN_INT();
    RUN_TEST(testConstructEEPROMStorage);
    RUN_TEST(testLoadFromEEPROM);
    RUN_TEST(testSetValue);
    RUN_TEST(testStoreToEEPROM);
    RUN_TEST(testGetSubArray);
    RUN_TEST(testGetArrayLength);    
    UNITY_END_INT();
}