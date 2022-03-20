#ifdef ARDUINO
#include <HardwareInterrupts.h>
#include <HardwarePins.h>
#include <SerialStream.h>
#include <MotorDriver.h>
#include <BasicEncoder.h>


#if 1
#define MOTOR_EN1 3
#define MOTOR_EN2 4
#define MOTOR_PWM 5

#define OPTO      16
#define ENCODER_2 15
#define ENCODER_1 14
#else
#define MOTOR_EN1 8
#define MOTOR_EN2 7
#define MOTOR_PWM 9

#define ENCODER_2 10
#define ENCODER_1 11
#define OPTO 12
#endif

MotorDriver driver;
HardwareInterrupts hwInterrupts;
HardwarePins hwPins;

class BasicEncoderStore: public BasicEncoderListener
{
private:
    volatile int32_t clickPosition = 0;
    volatile int16_t lapCount = 0;

public:
    void signalStepForwardISR()
    {        
        hwInterrupts.disableInterrupts();
        clickPosition++;
        hwInterrupts.enableInterrupts();
    }

    void signalHomingISR()
    {
        lapCount++;
    }

    int32_t getClickPosition()
    {
        int32_t rValue = 0;
        hwInterrupts.disableInterrupts();
        rValue = this->clickPosition;
        hwInterrupts.enableInterrupts();
        return rValue;
    }

    int32_t getLapCount()
    {
        int32_t rValue = 0;
        hwInterrupts.disableInterrupts();
        rValue = this->lapCount;
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

    delay(5000);

    driver.setMotorPWM( 0 );

    Log << PRINTVAR(listener.getClickPosition()) << PRINTVAR(listener.getLapCount()) << endl;    
}

void loop()
{
}
#endif