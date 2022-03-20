#include <unityEx.h>
#include <SerialStream.h>

void processMotorSpeedRegulator();
void runAllTestsMotorSpeedCommander();
void runAllTestGaitCommander();
void runAllTestsHomer();

void process()
{
    UNITY_BEGIN_EXT();  
    processMotorSpeedRegulator();              
    runAllTestsMotorSpeedCommander();    
    runAllTestGaitCommander();
    runAllTestsHomer();
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
