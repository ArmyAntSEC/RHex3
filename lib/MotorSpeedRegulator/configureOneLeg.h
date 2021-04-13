#ifndef _CONFIGUREPINS_H_
#define _CONFIGUREPINS_H_

#include <HomingEncoder.h>
#include "MotorDriver.h"
#include "RecurringEncoderWrapper.h"
#include "MotorSpeedRegulator.h"
#include "MotorSpeedCommander.h"

#define MOTOR_EN1 4
#define MOTOR_EN2 5
#define MOTOR_CS A0
#define ENCODER_2 2
#define ENCODER_1 3
#define OPTO 7
#define MOTOR_PWM 6

HomingEncoder encoder;
EncoderWrapperComputeSpeedTask encoderWrapperComputeSpeed ( &encoder );
RecurringEncoderWrapperHoming<0> encoderWrapperHoming ( &encoder );
MotorDriver driver;
MotorSpeedRegulator regulator;
MotorSpeedCommander commander;

void initOneLeg()
{
    //Init the driver
    driver.config( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM, MOTOR_CS );        

    //Initialize the encoder
    encoder.config<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );    
    
    //Intialize the regulator        
    regulator.config(&encoder, &driver, 0.2, 0, 0.01, 1 );    

    //Initialize the speed commander
    commander.config( &encoder, &driver, &regulator );

    
}
#endif