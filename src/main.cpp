#ifdef ARDUINO
#include <HardwareClock.h>
#include <SerialStream.h>
#include <TaskScheduler.h>
#include <TaskAwareDelay.h>
#include <RecurringTaskGroup.h>
#include <oneLeg.h>

#define MOTOR1_EN1 3
#define MOTOR1_EN2 4
#define MOTOR1_PWM 5
#define MOTOR1_ENCODER2 14
#define MOTOR1_ENCODER1 15
#define MOTOR1_OPTO 16
LegPinList leftLegPins = { MOTOR1_EN1, MOTOR1_EN2, MOTOR1_PWM, MOTOR1_ENCODER1, MOTOR1_ENCODER2, MOTOR1_OPTO };

#define MOTOR2_EN1 8
#define MOTOR2_EN2 7
#define MOTOR2_PWM 9
#define MOTOR2_ENCODER2 10
#define MOTOR2_ENCODER1 11
#define MOTOR2_OPTO 12
LegPinList rightLegPins = { MOTOR2_EN1, MOTOR2_EN2, MOTOR2_PWM, MOTOR2_ENCODER1, MOTOR2_ENCODER2, MOTOR2_OPTO };

OneLeg leftLeg;
OneLeg rightLeg;


HardwareClock hwClock;

TaskScheduler<1> sched;
RecurringTaskGroup<2> recurringGroup( 10*1000L );

TaskAwareDelay awareDelay(&hwClock, &sched);



void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Log << "Hello World!" << endl;  
  
  leftLeg.config<0>(&leftLegPins);
  leftLeg.setSpeedSetpoint( 3000 );
  rightLeg.config<1>(&rightLegPins);
  rightLeg.setSpeedSetpoint( 3000 );
  
  sched.addTask( &recurringGroup );
  recurringGroup.addTask( &leftLeg );
  recurringGroup.addTask( &rightLeg );
  
  Log << "Starting" << endl;
  awareDelay.delayMicros( 1000*1000L );
  Log << "Done: " << leftLeg.linPos.getLinearPosition() << endl;  
  Log << "Done: " << rightLeg.linPos.getLinearPosition() << endl;  

  leftLeg.driver.setMotorPWM( 0 );
  rightLeg.driver.setMotorPWM( 0 );
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
