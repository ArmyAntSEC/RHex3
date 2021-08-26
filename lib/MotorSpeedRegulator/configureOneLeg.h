#ifndef _CONFIGUREPINS_H_
#define _CONFIGUREPINS_H_

#include <HomingEncoder.h>
#include "MotorDriver.h"
#include "RecurringSpeedComputingEncoderWrapper.h"
#include "MotorSpeedRegulator.h"
#include "MotorSpeedCommander.h"
#include "LegController.h"
#include "SpeedToPowerConverter.h"

#define MOTOR_EN1 7
#define MOTOR_EN2 8
#define MOTOR_PWM 9

#define OPTO      10
#define ENCODER_2 11
#define ENCODER_1 12

HomingEncoder* encoder;
EncoderWrapperComputeSpeedTask encoderWrapperComputeSpeed;
MotorDriver driver;
MotorSpeedRegulator regulator;
MotorSpeedCommander commander;
LegController leg;
SpeedToPowerConverterProduction converter;


void initOneLeg()
{
    converter.initFromEEPROM();

    driver.config( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM );        
    
    encoder = HomingEncoderFactory::config<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );    

    encoderWrapperComputeSpeed.configure( encoder );
             
    regulator.config(encoder, &driver, &converter, 0.2, 0, 0.01, 1 );        
    
    commander.config( encoder, &driver, &regulator );
    
    leg.config( &commander );        
}
#endif