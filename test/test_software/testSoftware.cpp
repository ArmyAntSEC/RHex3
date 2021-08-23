#include <unity.h>

#include "testSpeedVsPowerConverter.h"
#include "testRotationPositionWithLap.h"
#include "testEEPROMStorage.h"

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    
    
    UNITY_BEGIN();    
    runAllEEPROMStorageTests();
    runAllTestsSpeedVsPowerConverter();
    runAllTestsRotationPositionWithLap();
    
    
    UNITY_END();
    
}

void loop() {
}