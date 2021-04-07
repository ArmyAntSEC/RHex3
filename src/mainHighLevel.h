#ifndef _MAINHIGHLEVEL_H_
#define _MAINHIGHLEVEL_H_

#include "main.h"
#include "MotorSpeedCommander.h"
#include "motorScheduler.h"

MotorSpeedCommander motorSpeedCommander ( &encoder, &driver, &regulator, &dataLogger );
MotorScheduler scheduler( &motorSpeedCommander );

void setupDetails()
{    
    dataLogger.init(millis());
    recurring10ms.add( &motorSpeedCommander );      
    recurring10ms.add( &scheduler );
    recurring10ms.add( &dataLogger );    
    motorSpeedCommander.registerVariables();    
    motorSpeedCommander.init( millis(), 1500, 10000 );
    scheduler.init( millis() );
    dataLogger.sendHeaders(); 
}

#endif