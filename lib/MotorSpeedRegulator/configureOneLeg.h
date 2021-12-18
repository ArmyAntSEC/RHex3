#ifndef _CONFIGUREPINS_H_
#define _CONFIGUREPINS_H_

#include <HomingEncoder.h>
#include "MotorDriver.h"
#include "ComputeSpeedEncoderWrapperTask.h"
#include "MotorSpeedRegulator.h"
#include "MotorSpeedCommander.h"
#include "LegController.h"
#include "SpeedToPowerConverter.h"

//#define MOTOR1 1

#ifdef MOTOR1

#define MOTOR_EN1 3
#define MOTOR_EN2 4
#define MOTOR_PWM 5

#define OPTO      16
#define ENCODER_2 15
#define ENCODER_1 14

#else

#define MOTOR_EN1 7
#define MOTOR_EN2 8
#define MOTOR_PWM 9

#define OPTO      10
#define ENCODER_2 11
#define ENCODER_1 12

#endif

HomingEncoder* encoder;
ComputeSpeedEncoderWrapperTask encoderWrapperComputeSpeed;
MotorDriver driver;
MotorSpeedRegulator regulator;
MotorSpeedCommander commander;
LegController leg;
SpeedToPowerConverter converter;


void initOneLeg()
{
    converter.initFromEEPROM();

    driver.config( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM );        
    
    HomingEncoderFactory homingEncoderFactory;
    encoder = homingEncoderFactory.config<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );    

    encoderWrapperComputeSpeed.configure( encoder );
             
    regulator.config(encoder, &driver, &converter, 0.2, 0, 0.01, 1 );        
    
    commander.config( encoder, &driver, &regulator );
    
    leg.config( &commander );        
}
#endif