#include <unity.h>

#include "testRotationPositionWithLaps.h"
#include "testVolatileRotationPositionWithLaps.h"
#include <HardwareInterface.h>

void process()
{
    runAllTestsRotationPositionWithLap();
    runAllTestsVolatileRotationPositionWithLap();         
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
