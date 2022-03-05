#include <unityEx.h>

void runAllTestsBasicEncoder();
void runAllTestsLinearPositionEncoder();
void runAllTestsSpeedComputer();
void runAllTestRotationalPosition();

void process()
{
    UNITY_BEGIN_EXT();
    runAllTestsBasicEncoder();
    runAllTestsLinearPositionEncoder();
    runAllTestsSpeedComputer();
    runAllTestRotationalPosition();
    UNITY_END_EXT();
}

#ifdef ARDUINO
#include <HardwareClock.h>
HardwareClock hwClock;

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    hwClock.delayMicroseconds(2000000);

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
