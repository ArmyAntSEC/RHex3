#include <HardwareClock.h>
#include <HardwareInterrupts.h>
#include <HardwarePins.h>

#include <LinearPositionalEncoder.h>
#include <BasicEncoder.h>
#include <SpeedComputer.h>
#include <MotorDriver.h>
#include <SpeedRegulator.h>
#include <RunnableInterface.h>

struct LegPinList
{
    int motorEnable1;
    int motorEnable2;
    int motorPWM;
    int encoder1;
    int encoder2;
    int homing;
};

struct OneLeg : public RunnableInterface
{
    HardwareInterrupts hwInterrupts;
    HardwarePins hwPins;
    HardwareClock hwClock;
    LinearPositionEncoder linPos;
    BasicEncoderFactory factory;
    BasicEncoder *encoder;
    MotorDriver driver;
    SpeedComputer speed;
    SpeedRegulator regulator;

    template <int EncoderChannel>
    void config( LegPinList* pinList )
    {
        linPos.config(&hwInterrupts);
        encoder = factory.config<EncoderChannel>(pinList->encoder1, pinList->encoder2, pinList->homing, &hwPins);
        encoder->addListener(&linPos);
        encoder->addListener(&speed);

        driver.config(pinList->motorEnable1, pinList->motorEnable2, pinList->motorPWM, &hwPins);
        //driver.setMotorPWM(128);

        speed.config(&hwClock, &hwInterrupts);

        regulator.config(&speed, &driver, 0.1, 0.015, 0, 10);
        
        regulator.start();

        linPos.forceHomed();
    }

    void setSpeedSetpoint( int speed )
    {
        regulator.setSetPoint( speed );
    }

    virtual void run(unsigned long _nowMicros)
    {
        regulator.run(_nowMicros);
    }
};
