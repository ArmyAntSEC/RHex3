#include <unityEx.h>
#include <SerialStream.h>

void processMotorSpeedRegulator();
void runAllTestsMotorSpeedCommander();
void runAllTestGaitCommander();

void process()
{
    UNITY_BEGIN_EXT();  
    processMotorSpeedRegulator();              
    runAllTestsMotorSpeedCommander();    
    runAllTestGaitCommander();
    UNITY_END_EXT();  
}

#ifdef ARDUINO
#include <Arduino.h>

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
