#include <unity.h>

#include "testSpeedVsPowerConverter.h"
#include "testRotationPositionWithLaps.h"
#include "testVolatileRotationPositionWithLaps.h"
#include <HardwareInterface.h>

void process()
{
    UNITY_BEGIN();        
    runAllTestsSpeedVsPowerConverter();
    runAllTestsRotationPositionWithLap();
    runAllTestsVolatileRotationPositionWithLap();        
    UNITY_END();
    

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