#include <Arduino.h>

#define MOTOR_EN1 4
#define MOTOR_EN2 5
#define MOTOR_CS A0
#define ENCODER_2 2
#define ENCODER_1 3
#define OPTO 7
#define MOTOR_PWM 6
#define UNCONNECTED_ANALOG A2

#include "HomingEncoder.h"
#include <TaskScheduler.h>
#include <SerialStream.h>
#include "MotorDriver.h"
#include "miniTests/SimpleMoveTest.h"
#include "CommandAndControll.h"

HomingEncoder encoder;
MotorDriver driver;

SimpleMoveTest simpleMoveTest( &encoder, &driver );

TaskScheduler sched;
CommandAndControll ctr ( &sched );

void setup()
{
  Serial.begin( 115200 );  

  pinMode ( MOTOR_EN1, OUTPUT );
  pinMode ( MOTOR_EN2, OUTPUT );
  pinMode ( MOTOR_PWM, OUTPUT );
  pinMode ( UNCONNECTED_ANALOG, INPUT );  

  randomSeed(analogRead(UNCONNECTED_ANALOG));

  encoder.init<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );
  driver.init( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM, MOTOR_CS );
  
  driver.setMotorPWM(0);   

  ctr.registerRemoteRoutine(&simpleMoveTest,1);
}

void loop()
{
  unsigned long int now = millis();
  ctr.run(now);
  sched.run();
}



