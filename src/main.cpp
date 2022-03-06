#ifdef ARDUINO
#include <Arduino.h>
#include <HardwareClock.h>
#include <RotationalPosition.h>
#include <BasicEncoder.h>
#include <LinearPositionalEncoder.h>
#include <SpeedComputer.h>
#include <SerialStream.h>

struct BasicEncoderListenerMock: public BasicEncoderListener
{
    volatile int stepCounter;
    volatile int homingCounter;

    virtual void signalStepForwardISR()
    {
        stepCounter++;    
    }

    virtual void signalHomingISR()
    {
        homingCounter++;
    }
};

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Serial.println("Hello World!");
}


void loop()
{  
}

#else

int main()
{

}

#endif
