#include <unity.h>

#include "testSpeedVsPowerConverter.h"
#include "testRotationPositionWithLaps.h"
#include "testVolatileRotationPositionWithLaps.h"
#include "testEEPROMStorage.h"

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    
    
    UNITY_BEGIN();    
    runAllEEPROMStorageTests();
    runAllTestsSpeedVsPowerConverter();
    runAllTestsRotationPositionWithLap();
    runAllTestsVolatileRotationPositionWithLap();
    
    
    UNITY_END();
    
}

void loop() {
}