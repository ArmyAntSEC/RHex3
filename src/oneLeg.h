#pragma once

#include <HardwarePins.h>
#include <BasicEncoder.h>
#include <LinearPositionalEncoder.h>
#include <RotationalPosition.h>
#include <SpeedComputer.h>
#include <MotorDriver.h>
#include <SpeedRegulator.h>
#include <MotorSpeedCommander.h>
#include <MotorDriver.h>

template <int N>
class OneLeg: public MotorSpeedCommanderInterface, public RunnableInterface
{
private:
    HardwarePins hwPins;
    BasicEncoder* encoder;
    LinearPositionEncoder linPos;
    SpeedComputer speedComp;
    RotationalPositionEncoder rotPos;
    SpeedRegulator speedRegulator;
    MotorSpeedCommander speedCommander;
    MotorDriver driver;


    
public:    
    OneLeg( int encoderPin1, int encoderPin2, int homingPin )
    {
        encoder = BasicEncoderFactory::config<N>(encoderPin1, encoderPin2, homingPin, &hwPins);
        encoder->addListener( &linPos );
        encoder->addListener ( &speedComp );
        rotPos.config( &linPos );        

        speedRegulator.config( &speedComp, &driver, 1, 1, 0, 10 );
        speedCommander.config( &rotPos, &speedRegulator, 3000 );
    }

    virtual void setGoal( RotationalPosition _pos, unsigned long _time )
    {
        speedCommander.setGoal( _pos, _time );
    }

    virtual void run( unsigned long _nowMicros )
    {
        speedCommander.run( _nowMicros );
        speedRegulator.run( _nowMicros );
    }
    
};