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
template <int EncoderChannel>
struct OneLeg : public RunnableInterface
{
    HardwareInterrupts* hwInterrupts;
    HardwarePins* hwPins;
    HardwareClock* hwClock;
    LinearPositionEncoder linPos;
    BasicEncoderFactory factory;
    BasicEncoder *encoder;
    MotorDriver driver;
    SpeedComputer speed;
    SpeedRegulator regulator;

    OneLeg( HardwareInterrupts* _hwInterrupts, HardwarePins* _hwPins, HardwareClock* _hwClock ):
        hwInterrupts(_hwInterrupts), hwPins(_hwPins), hwClock(_hwClock),
        linPos(_hwInterrupts), speed(_hwClock, _hwInterrupts)
    {}
    
    void config( LegPinList* pinList )
    {        
        encoder = factory.config<EncoderChannel>(pinList->encoder1, pinList->encoder2, pinList->homing, hwPins);
        encoder->addListener(&linPos);
        encoder->addListener(&speed);

        driver.config(pinList->motorEnable1, pinList->motorEnable2, pinList->motorPWM, hwPins);

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
