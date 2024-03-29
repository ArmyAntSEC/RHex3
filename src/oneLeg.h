#include <HardwareClock.h>
#include <HardwareInterrupts.h>
#include <HardwarePins.h>

#include <LinearPositionalEncoder.h>
#include <BasicEncoder.h>
#include <SpeedComputer.h>
#include <MotorDriver.h>
#include <SpeedRegulator.h>
#include <RunnableInterface.h>
#include <RotationalPosition.h>
#include <MotorSpeedCommander.h>
#include <Homer.h>
#include <LegCommandReceiver.h>
#include <EEPROMStorage.h>

struct LegPinList
{
    int16_t motorEnable1;
    int16_t motorEnable2;
    int16_t motorPWM;
    int16_t encoder1;
    int16_t encoder2;
    int16_t homing;
};

template <int16_t EncoderChannel>
struct OneLeg : public RunnableInterface, public MotorSpeedCommanderInterface
{
    HardwareInterrupts *hwInterrupts;
    HardwarePins *hwPins;
    HardwareClock *hwClock;
    LinearPositionEncoder linPos;
    RotationalPositionEncoder rotPos;
    BasicEncoderFactory factory;
    BasicEncoder *encoder;
    MotorDriver driver;
    SpeedComputer speed;
    SpeedRegulator regulator;
    MotorSpeedCommander commander;
    Homer legHomer;

    OneLeg(HardwareInterrupts *_hwInterrupts, HardwarePins *_hwPins, HardwareClock *_hwClock)
        : hwInterrupts(_hwInterrupts), hwPins(_hwPins), hwClock(_hwClock),
          linPos(_hwInterrupts), speed(_hwClock, _hwInterrupts), rotPos(&linPos),
          commander(&rotPos, &regulator), legHomer(&driver, &linPos)
    {
    }

    void initLegOffsets(int legIndex)
    {
        HardwareEEPROM rawEeprom;
        EEPROMStorage storage(&rawEeprom);
        int16_t legOffset = storage.readIntFromIndex(legIndex + 1);
        linPos.setOffset(legOffset);
        Log << "Offset: " << legOffset << endl;
    }

    void config(LegPinList *pinList)
    {
        encoder = factory.config<EncoderChannel>(pinList->encoder1, pinList->encoder2, pinList->homing, hwPins);
        encoder->addListener(&linPos);
        encoder->addListener(&speed);

        driver.config(pinList->motorEnable1, pinList->motorEnable2, pinList->motorPWM, hwPins);

        // Wait with integator term for now.
        regulator.config(&speed, &driver, 0.1, 0.015, 0, 10);

        linPos.forceHomed();
    }

    virtual void run(int32_t _nowMicros)
    {
        regulator.run(_nowMicros);
        commander.run(_nowMicros);
        legHomer.run(_nowMicros);
    }

    virtual void setGoal(MotorCommanderGoal goal, int32_t nowMicros)
    {
        Log << "Setting goal: Clicks: " << goal.targetPositionClicks << " Time: " << goal.targetRelativeTimeMicros << endl;
        commander.setGoal(goal, nowMicros);
    }

    void stop()
    {
        commander.stop();
        regulator.stop();
        legHomer.stop();
        driver.setMotorPWM(0);
    }

    void warmUpLegs(TaskAwareDelay *awareDelay)
    {
        regulator.setSetPoint(3500);
        regulator.start();

        Log << "Starting" << endl;

        awareDelay->delayMicros(1000 * 1000L);
        stop();
        Log << "Left Leg: " << linPos.getLinearPosition() << endl;
    }

    void doHoming(TaskAwareDelay *awareDelay)
    {
        Log << "Left leg homing started:" << PRINTVAR(linPos.isHomed()) << endl;
        legHomer.start();
        awareDelay->delayMicros(2e6L);
        stop();
        Log << "Left leg homing ended" << PRINTVAR(linPos.getLinearPosition()) << endl;
    }

    void goTo(TaskAwareDelay *awareDelay, int32_t pos)
    {
        Log << "Going to zero" << endl;
        int32_t nowMicros = hwClock->getMicrosecondsSinceBoot();
        MotorCommanderGoal goal;
        goal.targetPositionClicks = pos;
        goal.targetRelativeTimeMicros = 2e6L;
        commander.setGoal(goal, nowMicros);
        commander.start();
        regulator.start();
        awareDelay->delayMicros(2e6L);
        stop();
        RotationalPosition rotPos(linPos.getLinearPosition());
        Log << "Arrived at zero" << PRINTVAR(rotPos.getClicks()) << endl;
    }
};
