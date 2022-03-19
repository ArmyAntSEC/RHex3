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

HardwareClock hwClock;
HardwareInterrupts hwInterrupts;
HardwarePins hwPins;

OneLeg<0> leftLeg(&hwInterrupts, &hwPins, &hwClock);
OneLeg<1> rightLeg(&hwInterrupts, &hwPins, &hwClock);

TaskScheduler sched( &hwClock );
RecurringTaskGroup<2> recurringGroup( 10*1000L );

TaskAwareDelay awareDelay(&hwClock, &sched);

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Log << "Hello World!" << endl;  

  IdleCounter* idleCounter = sched.getIdleCounterObject();  
  idleCounter->Run1000IdleTaskToCalibrateAndGetMaxIdleCountsPerSecond();  
  
  leftLeg.config(&leftLegPins);
  leftLeg.setSpeedSetpoint( 4000 );
  rightLeg.config(&rightLegPins);
  rightLeg.setSpeedSetpoint( 4000 );
  
  sched.addTask( &recurringGroup );
  recurringGroup.addTask( &leftLeg );
  recurringGroup.addTask( &rightLeg );
  
  Log << "Starting" << endl;
  
  awareDelay.delayMicros( 1000*1000L );
  Log << "Left Leg: " << leftLeg.linPos.getLinearPosition() << endl;  
  Log << "Right Leg: " << rightLeg.linPos.getLinearPosition() << endl;  
  Log << "CPU Idle fraction: " << idleCounter->getCPUFactorPercent() << endl;  
  
  leftLeg.driver.setMotorPWM( 0 );
  rightLeg.driver.setMotorPWM( 0 );

  int16_t startSlow = 0;
  int16_t endSlow = 800;
  
  leftLeg.commander.start();
  
  Log << "Doing the fast move" << endl;
  leftLeg.setGoal( startSlow, hwClock.getMicrosecondsSinceBoot() + 1000*1000L );
  awareDelay.delayMicros( 1000*1000L );
  
  Log << "Doing the slow move" << endl;
  leftLeg.setGoal( endSlow, hwClock.getMicrosecondsSinceBoot() + 1000*1000L );
  awareDelay.delayMicros( 1000*1000L );
  
  Log << "Doing the fast move a second time" << endl;
  leftLeg.setGoal( startSlow, hwClock.getMicrosecondsSinceBoot() + 1000*1000L );
  awareDelay.delayMicros( 1000*1000L );
  
  leftLeg.driver.setMotorPWM( 0 );
  rightLeg.driver.setMotorPWM( 0 );
  Log << "Left Leg: " << leftLeg.linPos.getLinearPosition() << endl;  
  Log << "Exit" << endl;
}


void loop()
{
  uint32_t nowMicros = micros();
  //sched.run(nowMicros);  
}

#else

int16_t main()
{ 
    
}

#endif
