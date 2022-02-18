#include <unity.h>

#include <HardwareInterface.h>
#include "testBasicEncoder.h"
#include "testLinearPositionEncoder.h"
#include "testSpeedComputer.h"
#include "testRotationalPosition.h"

void setUp(void) {
    HardwareInterface::resetMicrosecondsSinceBoot();
}

void process()
{
    runAllTestsBasicEncoder();
    runAllTestsLinearPositionEncoder();
    runAllTestsSpeedComputer();
    runAllTestRotationalPosition();
}

#ifdef ARDUINO
void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    HardwareInterface::delayForMilliseconds(2000);

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
