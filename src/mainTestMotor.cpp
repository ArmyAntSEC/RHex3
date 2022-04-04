#ifdef ARDUINO
#include <HardwareInterrupts.h>
#include <HardwarePins.h>
#include <SerialStream.h>
#include <MotorDriver.h>
#include <BasicEncoder.h>


#if 0
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

struct BasicEncoderStore: public BasicEncoderListener
{    
    volatile int32_t clickPosition = 0;
    volatile int16_t lapCount = 0;
    volatile int16_t positionAtHoming;
    volatile int32_t timeAtHoming;
    volatile bool newLap = false;

    void signalStepForwardISR()
    {                
        clickPosition++;        
    }

    void signalHomingISR()
    {        
        positionAtHoming = clickPosition;
        timeAtHoming = millis();
        newLap = true;
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

    void PrintIfNewLap()
    {
        bool newLapExists = false;
        int32_t newLapTime;
        static int32_t oldLapTime;
        int32_t newLapPos;
        static int32_t oldLapPos;

        hwInterrupts.disableInterrupts();
        if ( newLap && lapCount > 0 ) {
            newLapExists = true;
            newLapTime = timeAtHoming;            
            newLapPos = positionAtHoming;            
            newLap = false;
        }          
        hwInterrupts.enableInterrupts();
        
        if ( newLapExists )
        {
            Log << "Position at lap " << lapCount << ": " << newLapPos;
            SerialLog << " Lap length: " << newLapPos - oldLapPos;
            SerialLog << " Lap time: " << newLapTime - oldLapTime;
            SerialLog << endl;

            oldLapPos = newLapPos;
            oldLapTime = newLapTime;
        }
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

    driver.setMotorPWM( 128 );

    int32_t endTime = millis() + 10000;
    while ( millis() < endTime ) {
        listener.PrintIfNewLap();
    }

    driver.setMotorPWM( 0 );

    
}

void loop()
{
}
#endif