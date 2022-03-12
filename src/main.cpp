#ifdef ARDUINO
#include <HardwareClock.h>
#include <HardwareInterrupts.h>
#include <HardwarePins.h>
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

HardwareClock hwClock;
HardwareInterrupts hwInterrupts;
HardwarePins hwPins;

TaskScheduler<1> sched;
RecurringTaskGroup<1> recurringGroup( 10*1000L );

TaskAwareDelay awareDelay(&hwClock, &sched);

LinearPositionEncoder linPos;
BasicEncoderFactory factory;
BasicEncoder* encoder;
MotorDriver driver;
SpeedComputer speed;
SpeedRegulator regulator;

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Log << "Hello World!" << endl;  
  
  sched.addTask( &recurringGroup );
  linPos.config(&hwInterrupts);
  encoder = factory.config<0>(MOTOR1_ENCODER1, MOTOR1_ENCODER2, MOTOR1_OPTO, &hwPins );
  encoder->addListener( &linPos );
  encoder->addListener( &speed );

  driver.config( MOTOR1_EN1, MOTOR1_EN2, MOTOR1_PWM, &hwPins );
  driver.setMotorPWM( 128 );

  speed.config( &hwClock, &hwInterrupts );

  regulator.config ( &speed, &driver, 1, 0, 0, 10 );
  recurringGroup.addTask( &regulator );
  regulator.setSetPoint( 1000 );
  regulator.start();

  Log << "Starting" << endl;
  awareDelay.delayMicros( 1000*1000L );
  Log << "Done: " << linPos.getLinearPosition() << endl;

  driver.setMotorPWM( 0 );
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
