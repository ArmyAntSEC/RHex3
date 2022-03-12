#include <HardwareClock.h>
#include <HardwareInterrupts.h>
#include <HardwarePins.h>

#include <LinearPositionalEncoder.h>
#include <BasicEncoder.h>
#include <SpeedComputer.h>
#include <MotorDriver.h>
#include <SpeedRegulator.h>
#include <RunnableInterface.h>

#define MOTOR1_EN1 3
#define MOTOR1_EN2 4
#define MOTOR1_PWM 5
#define MOTOR1_OPTO 16
#define MOTOR1_ENCODER2 15
#define MOTOR1_ENCODER1 14

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

    void config()
    {
        linPos.config(&hwInterrupts);
        encoder = factory.config<0>(MOTOR1_ENCODER1, MOTOR1_ENCODER2, MOTOR1_OPTO, &hwPins);
        encoder->addListener(&linPos);
        encoder->addListener(&speed);

        driver.config(MOTOR1_EN1, MOTOR1_EN2, MOTOR1_PWM, &hwPins);
        driver.setMotorPWM(128);

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
