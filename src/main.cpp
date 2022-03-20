#ifdef ARDUINO
#include <HardwareClock.h>
#include <SerialStream.h>
#include <TaskScheduler.h>
#include <TaskAwareDelay.h>
#include <RecurringTaskGroup.h>
#include <oneLeg.h>
#include <GaitCommander.h>

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
RecurringTaskGroup<3> recurringGroup( 10*1000L );

TaskAwareDelay awareDelay(&hwClock, &sched);

GaitCommander<2> gaitCommander;

LegCommandSequence leftLegSequence(&leftLeg);
LegCommandSequence rightLegSequence(&rightLeg);

void warmUpLegs()
{
  //Warm things up a bit.
  leftLeg.config(&leftLegPins);  
  leftLeg.setSpeedSetpoint( 3500 );
  leftLeg.regulator.start();
  
  rightLeg.config(&rightLegPins);  
  rightLeg.setSpeedSetpoint( 3500 );
  rightLeg.regulator.start();
  
  
  Log << "Starting" << endl;
  
  awareDelay.delayMicros( 1000*1000L );
  Log << "Left Leg: " << leftLeg.linPos.getLinearPosition() << endl;  
  Log << "Right Leg: " << rightLeg.linPos.getLinearPosition() << endl;    
  
  leftLeg.stop();
  rightLeg.stop();
  //End warmup
}

void doHoming()
{
  Log << "Left leg homing started:" << PRINTVAR(leftLeg.linPos.isHomed()) << endl;  
  leftLeg.legHomer.start();  
  awareDelay.delayMicros( 2e6L );
  leftLeg.stop();
  Log << "Left leg homing ended" << endl;
}

void goToZero()
{
  Log << "Going to zero" << endl;
  MotorSpeedCommanderInterface::LegCommand goal;
  goal.targetPositionClicks = 0;
  goal.targetTimeMicros = micros() + 2e6L;
  leftLeg.commander.setGoal(goal);
  leftLeg.commander.start();
  leftLeg.regulator.start();
  awareDelay.delayMicros( 2e6L );
  leftLeg.stop();
  RotationalPosition rotPos(leftLeg.linPos.getLinearPosition());
  Log << "Arrived at zero" << PRINTVAR(rotPos.getClicks()) << endl;

}

void configLegGait()
{
  int16_t slowStartPos = 0;
  int16_t slowTimePercent = 50;
  int16_t slowLength = 1000;
  int32_t period = 15e5;

  leftLegSequence.config( slowStartPos, slowTimePercent, slowLength, period );
}

void startWalking()
{
  Log << "Start walking" << endl;  
  gaitCommander.addLegSchedule( &leftLegSequence );    
  recurringGroup.addTask( &gaitCommander );
  
  leftLeg.commander.start();
  leftLeg.regulator.start();

  awareDelay.delayMicros( 6e6L );   
  
  leftLeg.stop();  
  
  Log << "Done walking" << endl;
}

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Log << "*************************" << endl << "Hello World!" << endl;  
  
  IdleCounter* idleCounter = sched.getIdleCounterObject();    
  idleCounter->Run1000IdleTaskToCalibrateAndGetMaxIdleCountsPerSecond();  
  Log << "Max idle counts per second: " << idleCounter->getMaxIdleCountsPerSecond() << endl;
  
  sched.addTask( &recurringGroup );
  recurringGroup.addTask( &leftLeg );
  recurringGroup.addTask( &rightLeg );
    
  warmUpLegs();   
  Log << "CPU Idle fraction: " << idleCounter->getCPUFactorPercent() << "%" << endl;   

  doHoming();
  Log << "CPU Idle fraction: " << idleCounter->getCPUFactorPercent() << "%" << endl;   

  for ( int i = 0; i < 5; i++ ) {
    goToZero();
    Log << "CPU Idle fraction: " << idleCounter->getCPUFactorPercent() << "%" << endl;   
  }
  
  Log << "Exit" << endl;
}


void loop()
{  
}

#else

int16_t main()
{ 
    
}

#endif
