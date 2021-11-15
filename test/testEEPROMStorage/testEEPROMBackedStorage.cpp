#include <unity.h>

#include "MockEEPROM.h"

#define private public
#include <EEPROMBackedArray.h>

void testConstructEEPROMStorage()
{
    EEPROMBackedArray<0,10,2> array;
    TEST_ASSERT_EACH_EQUAL_INT( 0, array.values,10*2 );
}

void testLoadFromEEPROM()
{
    int expected[2][2] = {{1, 2}, {3, 4}};
    MockEEPROMStorage eeprom;
    eeprom.rawData = (int*)expected;

    EEPROMBackedArray<0,2,2> array;
    array.setEEPROMStorageInterface( &eeprom );
    array.loadFromEEPROM();
    
    TEST_ASSERT_EQUAL_INT_ARRAY( expected, array.values, 2*2 );
}

void testSetValue()
{
    EEPROMBackedArray<0,2,2> array;
    array.setValue(1,0,42);
    TEST_ASSERT_EQUAL_INT( 42, array.values[1][0] );
}

void testStoreToEEPROM()
{
    int expected[2][2] = {{0, 0}, {0, 0}};
    MockEEPROMStorage eeprom;
    eeprom.rawData = (int*)expected;
    EEPROMBackedArray<0,2,2> array;
    array.setEEPROMStorageInterface( &eeprom );    
    array.setValue(1,0,42);
    
    array.storeToEEPROM();
    
    TEST_ASSERT_EQUAL_INT( 42, expected[1][0] );
}

void testGetSubArray()
{
    int expected[2][2] = {{1, 2}, {3, 4}};
    MockEEPROMStorage eeprom;
    eeprom.rawData = (int*)expected;

    EEPROMBackedArray<0,2,2> array;
    array.setEEPROMStorageInterface( &eeprom );
    array.loadFromEEPROM();

    int const * subArray = array.getSubArray(1);

    TEST_ASSERT_EQUAL_INT_ARRAY( expected[1], subArray, 2 );           
}

void processEEPROMBackedArrayStorage()
{
    RUN_TEST(testConstructEEPROMStorage);
    RUN_TEST(testLoadFromEEPROM);
    RUN_TEST(testSetValue);
    RUN_TEST(testStoreToEEPROM);
    RUN_TEST(testGetSubArray);
}