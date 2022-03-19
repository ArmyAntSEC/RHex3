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
