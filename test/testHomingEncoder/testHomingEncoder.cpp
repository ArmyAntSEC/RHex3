#include <unity.h>

#include <HardwareInterface.h>
#include "testHomingEncoderISRs.h"
#include "testHomingEncoderOverflowAndHoming.h"
#include "testHomingEncoderComputeSpeed.h"
#include "testBasicEncoder.h"

void setUp(void) {
    HardwareInterface::resetMicrosecondsSinceBoot();
}

void process()
{
    UNITY_BEGIN();  
    runAllTestsHomingEncoderISR();      
    runAllTestsHomingEncoderOverflowAndHoming();
    runAllTestsHomingEncoderComputeSpeed();
    runAllTestsBasicEncoder();
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
