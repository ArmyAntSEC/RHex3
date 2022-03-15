#include <unityEx.h>

void processMotorSpeedRegulator();
void runAllTestsMotorSpeedCommander();

void process()
{
    UNITY_BEGIN_EXT();  
    processMotorSpeedRegulator();              
    runAllTestsMotorSpeedCommander();
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
