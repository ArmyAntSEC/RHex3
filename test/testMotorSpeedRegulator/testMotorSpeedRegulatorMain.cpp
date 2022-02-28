#include <unityEx.h>

void processMotorSpeedRegulator();
void processInterpolator();
void processSpeedToPowerConverter();

void process()
{
    UNITY_BEGIN_EXT();  
    processMotorSpeedRegulator();    
    processInterpolator();
    processSpeedToPowerConverter();    
    UNITY_END_EXT();  
}

#ifdef ARDUINO
#include <Arduino.h>

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

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
