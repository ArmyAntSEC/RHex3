#include <Arduino.h>
#include <SerialStream.h>
#include <HardwareInterface.h>
#include <MotorDriver.h>
#include <BasicEncoder.h>

#define MOTOR_EN1 3
#define MOTOR_EN2 4
#define MOTOR_PWM 5

#define OPTO      16
#define ENCODER_2 15
#define ENCODER_1 14

MotorDriver driver;

class EncoderListener: public BasicEncoderListener
{
private:
    volatile long clickPosition = 0;

public:
    void signalStepForwardISR()
    {
        HardwareInterface::disableInterrupts();
        clickPosition++;
        HardwareInterface::enableInterrupts();
    }

    void signalHomingISR()
    {

    }

    long getClickPosition()
    {
        long rValue = 0;
        HardwareInterface::disableInterrupts();
        rValue = this->clickPosition;
        HardwareInterface::enableInterrupts();
        return rValue;
    }
};

EncoderListener listener;
BasicEncoderFactory encoderFactory;
BasicEncoder* encoder;

void setup()
{
    Serial.begin(115200);
    
    while (!Serial) {}

    Serial.println("Hello World!");    

    driver.config( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM );
    
    encoder = encoderFactory.config<0>( ENCODER_1, ENCODER_2, OPTO, &listener );

    driver.setMotorPWM( 128 );

    delay(1000);

    driver.setMotorPWM( 0 );

    Serial.print( "Position: " );
    Serial.println( listener.getClickPosition() );
}

void loop()
{
}
