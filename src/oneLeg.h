#pragma once

#include <HardwarePins.h>
#include <HardwareInterrupts.h>
#include <BasicEncoder.h>
#include <LinearPositionalEncoder.h>
#include <RotationalPosition.h>
#include <SpeedComputer.h>
#include <MotorDriver.h>
#include <SpeedRegulator.h>
#include <MotorSpeedCommander.h>
#include <MotorDriver.h>

struct MotorPinDefinition
{
    int EncoderOne;
    int EncoderTwo;
    int EncoderHoming;
    int MotorOne;
    int MotorTwo;
    int MotorPWM;
};

template <int N>
class OneLeg: public MotorSpeedCommanderInterface, public RunnableInterface
{
public:
    HardwarePins hwPins;
    HardwareInterrupts hwInterrupts;
    BasicEncoder* encoder;
    LinearPositionEncoder linPos;
    SpeedComputer speedComp;
    RotationalPositionEncoder rotPos;
    SpeedRegulator speedRegulator;
    MotorSpeedCommander speedCommander;
    MotorDriver driver;
public:    
    OneLeg( const MotorPinDefinition& motorPinDefinition )
    {
        encoder = BasicEncoderFactory::config<N>(
            motorPinDefinition.EncoderOne, 
            motorPinDefinition.EncoderTwo, 
            motorPinDefinition.EncoderHoming, 
            &hwPins
        );

        driver.config( 
            motorPinDefinition.MotorOne, 
            motorPinDefinition.MotorTwo,
            motorPinDefinition.MotorPWM,
            &hwPins
        );
        
        encoder->addListener( &linPos );
        encoder->addListener ( &speedComp );
        
        linPos.config( &hwInterrupts );
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