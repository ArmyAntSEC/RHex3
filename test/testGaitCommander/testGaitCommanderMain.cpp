#include <unityEx.h>
#include <SerialStream.h>

void runAllTestGaitCommander();
void runAllTestSpeedCommanderGoalSerializer();
void runAllTestsLegCommandSender();

void process()
{
    UNITY_BEGIN_EXT();  
    runAllTestGaitCommander();                  
    runAllTestSpeedCommanderGoalSerializer();
    runAllTestsLegCommandSender();
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
