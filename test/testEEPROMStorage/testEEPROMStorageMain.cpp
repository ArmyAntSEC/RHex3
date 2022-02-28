#include <unityEx.h>
#include <HardwareClock.h>

void processEEPROMStorage();

void processEEPROMBackedArrayStorage();

void process()
{
    UNITY_BEGIN_EXT();
    processEEPROMStorage();   
    processEEPROMBackedArrayStorage();         
    UNITY_END_EXT();
}

#ifdef ARDUINO

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

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
