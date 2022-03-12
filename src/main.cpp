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
#define MOTOR1_ENCODER1 14
#define MOTOR1_ENCODER2 15
#define MOTOR1_OPTO 16

LegPinList leftLegPins = { MOTOR1_EN1, MOTOR1_EN2, MOTOR1_PWM, MOTOR1_ENCODER1, MOTOR1_ENCODER2, MOTOR1_OPTO };
OneLeg leftLeg;

HardwareClock hwClock;

TaskScheduler<1> sched;
RecurringTaskGroup<1> recurringGroup( 10*1000L );

TaskAwareDelay awareDelay(&hwClock, &sched);



void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Log << "Hello World!" << endl;  
  
  leftLeg.config(&leftLegPins);
  leftLeg.setSpeedSetpoint( 1000 );
  
  sched.addTask( &recurringGroup );
  recurringGroup.addTask( &leftLeg );
  
  Log << "Starting" << endl;
  awareDelay.delayMicros( 1000*1000L );
  Log << "Done: " << leftLeg.linPos.getLinearPosition() << endl;  

  leftLeg.driver.setMotorPWM( 0 );
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
