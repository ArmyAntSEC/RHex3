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
