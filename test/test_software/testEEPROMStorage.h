#include <unity.h>
#include <EEPROMStorage.h>

void testWriteAndReadEEPROM()
{
    uint8_t testAddress = 0;
    int testValue1 = 4;
    int testValue2 = 8;    

    EEPROMStorage::writeIntToAddress(testAddress,testValue1);
    int testValueRead1 = EEPROMStorage::readIntFromAddress(testAddress);
    TEST_ASSERT_EQUAL ( testValue1, testValueRead1 );

    EEPROMStorage::writeIntToAddress(testAddress,testValue2);
    int testValueRead2 = EEPROMStorage::readIntFromAddress(testAddress);
    TEST_ASSERT_EQUAL ( testValue2, testValueRead2 );
}

void testReadAndWriteArray()
{
    uint8_t testAddress = 0;
    const int arrayLength = 2;
    int testValue1[arrayLength] = {4, 8};
    int testValue2[arrayLength] = {7, 9};

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

