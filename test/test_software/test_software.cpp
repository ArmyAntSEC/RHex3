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

    //Speed vs Power conversions
    RUN_TEST(testGetPowerforFreeSpeedExtrapolate);
    RUN_TEST(testGetPowerforFreeSpeedOnStep);
    RUN_TEST(testGetPowerforFreeSpeedInterpolate);
    
    //Interact betwen converter and EEPROM.
    RUN_TEST(testInitFromEEPROM);

    UNITY_END();
}

void loop() {
}