#ifdef ARDUINO
#include <HardwareClock.h>
#include <SerialStream.h>
#include <TaskScheduler.h>
#include <TaskAwareDelay.h>
#include <RecurringTaskGroup.h>
#include "oneLeg.h"

#define MOTOR1_EN1 3
#define MOTOR1_EN2 4
#define MOTOR1_PWM 5
#define MOTOR1_OPTO      16
#define MOTOR1_ENCODER2 15
#define MOTOR1_ENCODER1 14
MotorPinDefinition leftMotor = { 
  MOTOR1_ENCODER1, MOTOR1_ENCODER2, MOTOR1_OPTO,
  MOTOR1_EN1, MOTOR1_EN2, MOTOR1_PWM 
};

#define MOTOR2_EN1 6 /* Should be Pin 7, but not sure why this pin does not work. Is Pin 7 perhaps short-circuited?*/
#define MOTOR2_EN2 8
#define MOTOR2_PWM 9
#define MOTOR2_OPTO     10
#define MOTOR2_ENCODER2 11
#define MOTOR2_ENCODER1 12
MotorPinDefinition rightMotor = { 
  MOTOR2_ENCODER1, MOTOR2_ENCODER2, MOTOR2_OPTO,
  MOTOR2_EN1, MOTOR2_EN2, MOTOR2_PWM 
};

HardwareClock hwClock;

OneLeg<0> leftLeg(leftMotor);
OneLeg<1> rightLeg(rightMotor);

TaskScheduler<1> sched;
RecurringTaskGroup<2> recurringGroup( 10*1000L );

TaskAwareDelay awareDelay(&hwClock, &sched);

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Log << "Hello World!" << endl;  

  recurringGroup.addTask( &leftLeg );  
  recurringGroup.addTask( &rightLeg );  
  
  sched.addTask( &recurringGroup );    
  
  leftLeg.speedRegulator.setSetPoint(128); //Why does this not work?
  leftLeg.speedRegulator.start(); //This causes a crash.....
  rightLeg.driver.setMotorPWM(255);
  Log << "Start delay" << endl;
  awareDelay.delayMicros ( 1000*1000L );  
  Log << "End delay" << endl;
  leftLeg.driver.setMotorPWM(0);
  rightLeg.driver.setMotorPWM(0);
}


void loop()
{
  unsigned long nowMicros = micros();
  //sched.run(nowMicros);  
}

#else

int main()
{ 
    
}

#endif
