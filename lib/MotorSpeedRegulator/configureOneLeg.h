#ifndef _CONFIGUREPINS_H_
#define _CONFIGUREPINS_H_

#include <HomingEncoder.h>
#include "MotorDriver.h"
#include "RecurringSpeedComputingEncoderWrapper.h"
#include "MotorSpeedRegulator.h"
#include "MotorSpeedCommander.h"
#include "LegController.h"

#define MOTOR_EN1 7
#define MOTOR_EN2 8
#define MOTOR_PWM 9

#define OPTO      10
#define ENCODER_2 11
#define ENCODER_1 12

HomingEncoder encoder;
EncoderWrapperComputeSpeedTask encoderWrapperComputeSpeed ( &encoder );
MotorDriver driver;
MotorSpeedRegulator regulator;
MotorSpeedCommander commander;
LegController leg;


void initOneLeg()
{
    //Init the driver
    driver.config( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM );        

    //Initialize the encoder
    encoder.config<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );    
    
    //Intialize the regulator        
    regulator.config(&encoder, &driver, 0.2, 0, 0.01, 1 );    

    //Initialize the speed commander
    commander.config( &encoder, &driver, &regulator );

    //Initialize the leg stepper
    leg.config( &commander );        
}
#endif