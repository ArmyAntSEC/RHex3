#include <unityEx.h>
#include <SerialStream.h>

void processMotorSpeedRegulator();
void runAllTestsMotorSpeedCommander();
void runAllTestsHomer();
void runAllMotorGoalTests();

void process()
{
    UNITY_BEGIN_EXT();  
    processMotorSpeedRegulator();              
    runAllTestsMotorSpeedCommander();        
    runAllTestsHomer();
    runAllMotorGoalTests();
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
