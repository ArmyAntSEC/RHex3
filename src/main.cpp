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
LegPinList rightLegPins = { MOTOR1_EN1, MOTOR1_EN2, MOTOR1_PWM, MOTOR1_ENCODER1, MOTOR1_ENCODER2, MOTOR1_OPTO };

#define MOTOR2_EN1 8
#define MOTOR2_EN2 7
#define MOTOR2_PWM 9
#define MOTOR2_ENCODER2 10
#define MOTOR2_ENCODER1 11
#define MOTOR2_OPTO 12
LegPinList leftLegPins = { MOTOR2_EN1, MOTOR2_EN2, MOTOR2_PWM, MOTOR2_ENCODER1, MOTOR2_ENCODER2, MOTOR2_OPTO };

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



void configLegGait()
{
  int16_t slowStartPos = 0;
  int16_t slowTimePercent = 70;
  int16_t slowLength = 700;
  int32_t period = 1e6;

  leftLegSequence.config( slowStartPos, slowTimePercent, slowLength, period );
  rightLegSequence.config( slowStartPos, slowTimePercent, slowLength, period );
}

void startWalking()
{
  Log << "Start walking" << endl;  
  gaitCommander.addLegSchedule( &leftLegSequence );    
  gaitCommander.addLegSchedule( &rightLegSequence );    
  recurringGroup.addTask( &gaitCommander );
  
  leftLeg.commander.start();
  leftLeg.regulator.start();
  rightLeg.commander.start();
  rightLeg.regulator.start();

  awareDelay.delayMicros( 6e6L );   
  
  leftLeg.stop();  
  rightLeg.stop();  
  
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
  
  leftLeg.config(&leftLegPins);  
  rightLeg.config(&rightLegPins);  

  leftLeg.warmUpLegs(&awareDelay);   
  rightLeg.warmUpLegs(&awareDelay);   
  Log << "CPU Idle fraction: " << idleCounter->getCPUFactorPercent() << "%" << endl;   

  leftLeg.linPos.setOffset(0);
  rightLeg.linPos.setOffset(1500);

  leftLeg.doHoming(&awareDelay);
  rightLeg.doHoming(&awareDelay);
  Log << "CPU Idle fraction: " << idleCounter->getCPUFactorPercent() << "%" << endl;   

  for ( int i = 0; i < 4; i++ ) {
    leftLeg.goToZero(&awareDelay);
    rightLeg.goToZero(&awareDelay);
    Log << "CPU Idle fraction: " << idleCounter->getCPUFactorPercent() << "%" << endl;   
    awareDelay.delayMicros(1e6);
    Log << "CPU Idle fraction: " << idleCounter->getCPUFactorPercent() << "%" << endl;   
  }
  
  //configLegGait();
  //startWalking();

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
