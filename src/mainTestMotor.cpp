#ifdef ARDUINO
#include <HardwareInterrupts.h>
#include <HardwarePins.h>
#include <SerialStream.h>
#include <MotorDriver.h>
#include <BasicEncoder.h>



#define MOTOR1_EN1 3
#define MOTOR1_EN2 4
#define MOTOR1_PWM 5

#define OPTO1      16
#define ENCODER1_1 15
#define ENCODER1_2 14


#define MOTOR2_EN1 8
#define MOTOR2_EN2 7
#define MOTOR2_PWM 9

#define OPTO2 10
#define ENCODER2_1 11
#define ENCODER2_2 12


MotorDriver driver1;
MotorDriver driver2;
HardwareInterrupts hwInterrupts;
HardwarePins hwPins;

BasicEncoderFactory encoderFactory;
BasicEncoder* encoder1;
BasicEncoder* encoder2;

struct BasicEncoderStore: public BasicEncoderListener
{    
    volatile int32_t clickPosition = 0;
    volatile int16_t lapCount = 0;
    volatile int32_t positionAtHoming;
    volatile int32_t timeAtHoming;
    volatile bool newLap = false;
    int32_t oldLapTime;
    int32_t oldLapPos;

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

    void PrintIfNewLap( int ID )
    {
        bool newLapExists = false;
        int32_t newLapTime;        
        int32_t newLapPos;        

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
            Log << "ID: " << ID << " Position at lap " << lapCount << ": " << newLapPos;
            SerialLog << " Lap length: " << newLapPos - oldLapPos;
            SerialLog << " Lap time: " << newLapTime - oldLapTime;
            SerialLog << endl;

            oldLapPos = newLapPos;
            oldLapTime = newLapTime;
        }
    }
     
};

BasicEncoderStore listener1;
BasicEncoderStore listener2;

void setup()
{
    Serial.begin(115200);
    
    while (!Serial) {}

    Serial.println("Hello World!");    
    
    driver1.config( MOTOR1_EN1, MOTOR1_EN2, MOTOR1_PWM, &hwPins );
    driver2.config( MOTOR2_EN1, MOTOR2_EN2, MOTOR2_PWM, &hwPins );
    
    encoder1 = encoderFactory.config<0>( ENCODER1_1, ENCODER1_2, OPTO1, &hwPins );
    encoder1->addListener( &listener1 );
    
    encoder2 = encoderFactory.config<1>( ENCODER2_1, ENCODER2_2, OPTO2, &hwPins );
    encoder2->addListener( &listener2 );

    driver1.setMotorPWM( 128 );
    driver2.setMotorPWM( 128 );

    int32_t endTime = millis() + 10000;
    while ( millis() < endTime ) {        
        listener1.PrintIfNewLap(1);
        listener2.PrintIfNewLap(2);
    }

    driver1.setMotorPWM( 0 );
    driver2.setMotorPWM( 0 );    
}

void loop()
{
}
#endif