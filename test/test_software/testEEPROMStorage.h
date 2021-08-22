#include <unity.h>
#include <EEPROMStorage.h>

//TODO: Once we have Exception handling, create a test case for writing outside of the EEPROM array.

void testWriteAndReadEEPROM()
{
    uint8_t testIndex = 120; //Use the end of the array for testing
    int testValue1 = 654; 
    int testValue2 = 8;    

    EEPROMStorage::writeIntToIndex(testIndex,testValue1);
    int testValueRead1 = EEPROMStorage::readIntFromIndex(testIndex);
    TEST_ASSERT_EQUAL ( testValue1, testValueRead1 );

    EEPROMStorage::writeIntToIndex(testIndex,testValue2);
    int testValueRead2 = EEPROMStorage::readIntFromIndex(testIndex);
    TEST_ASSERT_EQUAL ( testValue2, testValueRead2 );
}

void testReadAndWriteArray()
{
    uint8_t testAddress = 120;
    const int arrayLength = 2;
    int testValue1[arrayLength] = {45, 843};
    int testValue2[arrayLength] = {743, 9};

    EEPROMStorage eepromStorage;

    eepromStorage.writeIntArrayToAddress(testAddress, testValue1, arrayLength);
    int testValueRead[2] = {};
    eepromStorage.readIntArrayFromAddress(testAddress, testValueRead, arrayLength);
    TEST_ASSERT_EQUAL ( testValue1[0], testValueRead[0] );
    TEST_ASSERT_EQUAL ( testValue1[1], testValueRead[1] );

    eepromStorage.writeIntArrayToAddress(testAddress, testValue2, arrayLength);    
    eepromStorage.readIntArrayFromAddress(testAddress, testValueRead, arrayLength);
    TEST_ASSERT_EQUAL ( testValue2[0], testValueRead[0] );
    TEST_ASSERT_EQUAL ( testValue2[1], testValueRead[1] );
}

