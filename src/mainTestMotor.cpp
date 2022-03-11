#ifdef ARDUINO
#include <HardwareInterrupts.h>
#include <HardwarePins.h>
#include <Arduino.h>
#include <SerialStream.h>
#include <MotorDriver.h>
#include <BasicEncoder.h>

#define MOTOR_EN1 3
#define MOTOR_EN2 4
#define MOTOR_PWM 5

#define OPTO      16
#define ENCODER_2 15
#define ENCODER_1 14

MotorDriver driver;
HardwareInterrupts hwInterrupts;
HardwarePins hwPins;

class BasicEncoderStore: public BasicEncoderListener
{
private:
    volatile long clickPosition = 0;

public:
    void signalStepForwardISR()
    {        
        hwInterrupts.disableInterrupts();
        clickPosition++;
        hwInterrupts.enableInterrupts();
    }

    void signalHomingISR()
    {

    }

    long getClickPosition()
    {
        long rValue = 0;
        hwInterrupts.disableInterrupts();
        rValue = this->clickPosition;
        hwInterrupts.enableInterrupts();
        return rValue;
    }
};

BasicEncoderStore listener;
BasicEncoderFactory encoderFactory;
BasicEncoder* encoder;

void setup()
{
    Serial.begin(115200);
    
    while (!Serial) {}

    Serial.println("Hello World!");    

    driver.config( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM, &hwPins );
    
    encoder = encoderFactory.config<0>( ENCODER_1, ENCODER_2, OPTO, &hwPins );
    encoder->addListener( &listener );

    driver.setMotorPWM( 64 );

    delay(10000);

    driver.setMotorPWM( 0 );

    Serial.print( "Position: " );
    Serial.println( listener.getClickPosition() );
}

void loop()
{
}
#endif