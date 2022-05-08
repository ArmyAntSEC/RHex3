#include <base.h>

#ifdef ARDUINO
#include <HardwareClock.h>
#include <SerialStream.h>
#include <TaskScheduler.h>
#include <TaskAwareDelay.h>
#include <RecurringTaskGroup.h>
#include <oneLeg.h>
#include <GaitCommander.h>
#include <LegCommandReceiver.h>
#include <EEPROMStorage.h>

#define MOTOR1_EN1 3
#define MOTOR1_EN2 4
#define MOTOR1_PWM 5

#define MOTOR1_OPTO 16
#define MOTOR1_ENCODER1 15
#define MOTOR1_ENCODER2 14

LegPinList rightLegPins = {MOTOR1_EN1, MOTOR1_EN2, MOTOR1_PWM, MOTOR1_ENCODER1, MOTOR1_ENCODER2, MOTOR1_OPTO};

#define MOTOR2_EN1 8
#define MOTOR2_EN2 7
#define MOTOR2_PWM 9

#define MOTOR2_OPTO 10
#define MOTOR2_ENCODER1 11
#define MOTOR2_ENCODER2 12

LegPinList leftLegPins = {MOTOR2_EN1, MOTOR2_EN2, MOTOR2_PWM, MOTOR2_ENCODER1, MOTOR2_ENCODER2, MOTOR2_OPTO};

HardwareClock hwClock;
HardwareInterrupts hwInterrupts;
HardwarePins hwPins;

OneLeg<0> leftLeg(&hwInterrupts, &hwPins, &hwClock);
OneLeg<1> rightLeg(&hwInterrupts, &hwPins, &hwClock);

I2CReceiverWrapper i2cWrapper;
LegCommandReceiver &receiver = LegCommandReceiver::getSingletonInstance();

TaskScheduler sched(&hwClock);
RecurringTaskGroup<3> recurringGroup(10 * 1000L);

TaskAwareDelay awareDelay(&hwClock, &sched);

void initI2C(I2CReceiverWrapper *i2cWrapper)
{
  HardwareEEPROM rawEeprom;
  EEPROMStorage storage(&rawEeprom);
  int16_t i2cID = storage.readIntFromIndex(0);
  i2cWrapper->config(i2cID);
  Log << "I2C ID: " << i2cID << endl;
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
  }
  Log << "*************************" << endl
      << "Hello World!" << endl;

  initI2C(&i2cWrapper);

  IdleCounter *idleCounter = sched.getIdleCounterObject();
  idleCounter->Run1000IdleTaskToCalibrateAndGetMaxIdleCountsPerSecond();
  Log << "Max idle counts per second: " << idleCounter->getMaxIdleCountsPerSecond() << endl;

  sched.addTask(&recurringGroup);
  recurringGroup.addTask(&leftLeg);
  recurringGroup.addTask(&rightLeg);

  receiver.config(&i2cWrapper, &leftLeg.commander, &rightLeg.commander, &hwInterrupts);
  recurringGroup.addTask(&receiver);

  leftLeg.config(&leftLegPins);
  rightLeg.config(&rightLegPins);

  leftLeg.initLegOffsets(0);
  rightLeg.initLegOffsets(1);

  Log << "Homing both legs" << endl;
  leftLeg.doHoming(&awareDelay);
  rightLeg.doHoming(&awareDelay);
  Log << "CPU Idle fraction: " << idleCounter->getCPUFactorPercent() << "%" << endl;

  Log << "Looping a full round to reach zero" << endl;
  for (int i = 0; i < 2; i++)
  {
    leftLeg.goTo(&awareDelay, i * 1792);
    rightLeg.goTo(&awareDelay, i * 1792);
  }

  Log << "CPU Idle fraction: " << idleCounter->getCPUFactorPercent() << "%" << endl;

  Log << "Setup done!" << endl;
  receiver.start();
}

void loop()
{
  sched.run(hwClock.getMicrosecondsSinceBoot());
}

#else

int16_t main()
{
}

#endif
