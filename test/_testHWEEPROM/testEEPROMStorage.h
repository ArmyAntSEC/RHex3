#include <unity.h>
#include <EEPROMStorage.h>

//TODO: Once we have Exception handling, create a test case for writing outside of the EEPROM array.

void testWriteAndReadEEPROM()
{
    uint8_t testIndex = 120; //Use the end of the array for testing
    int16_t testValue1 = 654; 
    int16_t testValue2 = 8;    

    EEPROMStorage::writeIntToIndex(testIndex,testValue1);
    int16_t testValueRead1 = EEPROMStorage::readIntFromIndex(testIndex);
    TEST_ASSERT_EQUAL ( testValue1, testValueRead1 );

    EEPROMStorage::writeIntToIndex(testIndex,testValue2);
    int16_t testValueRead2 = EEPROMStorage::readIntFromIndex(testIndex);
    TEST_ASSERT_EQUAL ( testValue2, testValueRead2 );
}

void testReadAndWriteArray()
{
    uint8_t testAddress = 120;
    const int16_t arrayLength = 2;
    int16_t testValue1[arrayLength] = {45, 843};
    int16_t testValue2[arrayLength] = {743, 9};

    EEPROMStorage eepromStorage;

    eepromStorage.writeIntArrayToAddress(testAddress, testValue1, arrayLength);
    int16_t testValueRead[2] = {};
    eepromStorage.readIntArrayFromAddress(testAddress, testValueRead, arrayLength);
    TEST_ASSERT_EQUAL ( testValue1[0], testValueRead[0] );
    TEST_ASSERT_EQUAL ( testValue1[1], testValueRead[1] );

    eepromStorage.writeIntArrayToAddress(testAddress, testValue2, arrayLength);    
    eepromStorage.readIntArrayFromAddress(testAddress, testValueRead, arrayLength);
    TEST_ASSERT_EQUAL ( testValue2[0], testValueRead[0] );
    TEST_ASSERT_EQUAL ( testValue2[1], testValueRead[1] );
}

void runAllEEPROMStorageTests()
{
    //EEPROM Storage    
    RUN_TEST(testWriteAndReadEEPROM);    
    RUN_TEST(testReadAndWriteArray);    
}