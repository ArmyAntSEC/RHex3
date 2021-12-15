#pragma once

//TODO: This file is just a sketch. 

#include <RotationPositionWithLaps.h>
#include <HomingEncoder.h>
#include "ComputeSpeedEncoderWrapperTask.h"
#include "MotorDriver.h"
#include "MotorSpeedRegulator.h"
#include "MotorSpeedCommander.h"
#include "LegController.h"

class CompleteLegPackage
{

    private:
    /*
    HomingEncoder* encoder;
    ComputeSpeedEncoderWrapperTask encoderWrapperComputeSpeed;
    MotorDriver driver;
    MotorSpeedRegulator regulator;
    MotorSpeedCommander commander;
    LegController leg;
    SpeedToPowerConverter converter;
    */
    public:

    void config()
    {
        /*
        converter.initFromEEPROM();

        driver.config( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM );        
        
        HomingEncoderFactory homingEncoderFactory;
        encoder = homingEncoderFactory.config<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );    

        encoderWrapperComputeSpeed.configure( encoder );
                
        regulator.config(encoder, &driver, &converter, 0.2, 0, 0.01, 1 );        
        
        commander.config( encoder, &driver, &regulator );
        
        leg.config( &commander );
        */        
    }
    
    RotationPositionWithLaps getPosition()
    {
        return RotationPositionWithLaps();
    }
};