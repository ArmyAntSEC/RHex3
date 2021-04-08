#ifndef _MAINHIGHLEVEL_H_
#define _MAINHIGHLEVEL_H_

#include "main.h"
#include "MotorSpeedCommander.h"
#include "motorScheduler.h"

MotorSpeedCommander motorSpeedCommander ( &encoder, &driver, &regulator, &dataLogger );
MotorScheduler scheduler( &motorSpeedCommander );

void setupDetails()
{        
    recurring10ms.add( &motorSpeedCommander );      
    recurring10ms.add( &scheduler );
    
    motorSpeedCommander.registerVariables();    
    motorSpeedCommander.init( millis(), 1500, 10000 );
    scheduler.init( millis() );    
}

#endif