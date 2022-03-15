#include <unityEx.h>

void runAllTestsBasicEncoder();
void runAllTestsLinearPositionEncoder();
void runAllTestsSpeedComputer();
void runAllTestRotationalPosition();
void runAllTestMeasurePerformance();

void process()
{
    UNITY_BEGIN_EXT();
    runAllTestsBasicEncoder();
    runAllTestsLinearPositionEncoder();
    runAllTestsSpeedComputer();
    runAllTestRotationalPosition();
    runAllTestMeasurePerformance();
    UNITY_END_EXT();
}

#ifdef ARDUINO
#include <HardwareClock.h>
HardwareClock hwClock;

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
