#include <Arduino.h>

#define MOTOR_EN1 4
#define MOTOR_EN2 5
#define MOTOR_CS A0
#define ENCODER_2 3
#define ENCODER_1 2
#define OPTO 7
#define MOTOR_PWM 6
#define UNCONNECTED_ANALOG A2

#include "HomingEncoder.h"
#include <TaskScheduler.h>
#include <SerialStream.h>
#include "MotorDriver.h"
#include "MotorSpeedController.h"

HomingEncoder encoder;

TaskScheduler sched;

MotorDriver driver;

MotorSpeedController controller ( 10 );

void testEncoderReliability()
{
  for ( int i = 0; i < 5; i++ ) {    
    encoder.unHome();

    int speed = -64;    
    driver.setMotorPWM(speed);      
    int randNum = random(100);
    delay ( 200 + randNum  );
    while ( !encoder.isHomed() ) {    
      encoder.isr_homing<0>(); 
      delay(10);   
    }
    driver.setMotorPWM(0);
    delay(1000);  
    Log << "Stop position: " << encoder.readCompensated() << endl;
    Log << "Position at homing: " << encoder.getPosAtLastHome() << endl;    
  }
}

void setup()
{
  Serial.begin( 115200 );  

  encoder.init<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );
  pinMode ( MOTOR_EN1, OUTPUT );
  pinMode ( MOTOR_EN2, OUTPUT );
  pinMode ( MOTOR_PWM, OUTPUT );
  pinMode ( UNCONNECTED_ANALOG, INPUT );

  digitalWrite ( MOTOR_EN1, 0 );
  digitalWrite ( MOTOR_EN2, 1 );

  randomSeed(analogRead(UNCONNECTED_ANALOG));
  Log << "Hello World: " << random(100) << endl;

  driver.init( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM, MOTOR_CS );

  testEncoderReliability();

  driver.setMotorPWM(0);
  
}


void loop()
{
  sched.run();
}



