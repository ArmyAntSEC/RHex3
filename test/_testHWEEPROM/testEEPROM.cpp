#include <unity.h>

#include "testEEPROMStorage.h"


void process()
{
    UNITY_BEGIN();        
    runAllEEPROMStorageTests();
    UNITY_END();    
}

#ifdef ARDUINO

#include <HardwareClock.h>

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    HardwareClock clock;
    clock.delayMicroseconds(2000);

    process();
}

void loop() {
}
#else
int main(void)
{
    process();
}
#endif
