#include <unityEx.h>
#include <HardwareClock.h>

void runAllRecurringTaskGroupTests();
void runAllTaskSchedulerTests();
void runAllTestsTaskAwareDelay();
void runAllTestsMeasureIdleTime();

void process()
{
    UNITY_BEGIN_EXT();
    runAllRecurringTaskGroupTests();
    runAllTaskSchedulerTests();
    runAllTestsTaskAwareDelay();
    runAllTestsMeasureIdleTime();
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
