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
    HardwareInterrupts* hwInterrupts;
    HardwarePins* hwPins;
    HardwareClock* hwClock;
    LinearPositionEncoder linPos;
    RotationalPositionEncoder rotPos;
    BasicEncoderFactory factory;
    BasicEncoder *encoder;
    MotorDriver driver;
    SpeedComputer speed;
    SpeedRegulator regulator;    
    MotorSpeedCommander commander;
    Homer legHomer;

    OneLeg( HardwareInterrupts* _hwInterrupts, HardwarePins* _hwPins, HardwareClock* _hwClock ):
        hwInterrupts(_hwInterrupts), hwPins(_hwPins), hwClock(_hwClock),
        linPos(_hwInterrupts), speed(_hwClock, _hwInterrupts), rotPos(&linPos), 
        commander(&rotPos, &regulator), legHomer(&driver,&linPos)
    {}
    
    void config( LegPinList* pinList )
    {        
        encoder = factory.config<EncoderChannel>(pinList->encoder1, pinList->encoder2, pinList->homing, hwPins);
        encoder->addListener(&linPos);
        encoder->addListener(&speed);

        driver.config(pinList->motorEnable1, pinList->motorEnable2, pinList->motorPWM, hwPins);
                
        //Wait with integator term for now.
        regulator.config(&speed, &driver, 0.1, 0*0.015, 0, 10); 
            
        linPos.forceHomed();                
    }

    void setSpeedSetpoint( int16_t speed )
    {
        regulator.setSetPoint( speed );
    }

    virtual void run(int32_t _nowMicros)
    {
        regulator.run(_nowMicros);
        commander.run(_nowMicros);        
        legHomer.run(_nowMicros);
    }

    virtual void setGoal( LegCommand goal )
    {
        Log << "Setting goal: Clicks: " << goal.targetPositionClicks << " Time: " << goal.targetTimeMicros << endl;
        commander.setGoal( goal );
    }

    void stop()
    {
        commander.stop();
        regulator.stop();        
        driver.setMotorPWM(0);
    }
};
