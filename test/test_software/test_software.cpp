#include <unity.h>

#include "test_speed_vs_power_lookup.h"
#include "testEEPROMStorage.h"

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    
    UNITY_BEGIN();
    
    //EEPROM Storage
    RUN_TEST(testWriteAndReadEEPROM);
    RUN_TEST(testReadAndWriteArray);

    //Speed vs Power conversions
    RUN_TEST(testGetPowerforFreeSpeedExtrapolate);
    RUN_TEST(testGetPowerforFreeSpeedOnStep);
    RUN_TEST(testGetPowerforFreeSpeedInterpolate);
    RUN_TEST(testGetFreeSpeedForPowerInterpolate);
    
    //Interact betwen converter and EEPROM.
    RUN_TEST(testInitSpeedToPowerConverterFromEEPROM);
    RUN_TEST(testSaveSpeedToPowerConverterToEEPROM);

    UNITY_END();
}

void loop() {
}