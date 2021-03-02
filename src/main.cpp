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
#include "MotorSpeedController.h"
#include "miniTests.h"

HomingEncoder encoder;

TaskScheduler sched;

MotorDriver driver;

MotorSpeedController controller;


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
  //Log << "Hello World: " << random(100) << endl;

  driver.init( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM, MOTOR_CS );

  testSpeedPDController(&encoder, &driver);
  driver.setMotorPWM(0);
  
}


void loop()
{
  sched.run();
}



