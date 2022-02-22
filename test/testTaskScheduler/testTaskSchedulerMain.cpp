#include <unity.h>

void runAllRecurringTaskGroupTests();
void runAllTaskSchedulerTests();
void runAllTestsTaskAwareDelay();

void process()
{
    runAllRecurringTaskGroupTests();
    runAllTaskSchedulerTests();
    runAllTestsTaskAwareDelay();
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