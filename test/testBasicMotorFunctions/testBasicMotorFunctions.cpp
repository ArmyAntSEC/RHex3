#include <Arduino.h>
#include <unity.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>

#include "testBasicMotorFunctions.h"
#include "testBasicMovement.h"
#include "testMotorRegulator.h"
#include "testMotorCommander.h"


RecurringTaskGroup<16> recurring10ms( 10 );


void setUp(void) {
    encoder.forceHomed();    
}

void tearDown(void) {
    driver.setMotorPWM(0);
    regulator.stop();
    commander.stop();    
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    initOneLeg();

    regulator.stop(); //Don't start the regulator from the get go.

    recurring10ms.add( &encoderWrapperComputeSpeed );    
    recurring10ms.add( &regulator );
    recurring10ms.add( &commander );
    recurring10ms.init( millis() );  
    sched.add( &recurring10ms );     
        
    UNITY_BEGIN();                        
    
    runAllBasicMovementTests();
    runAllMotorRegulatorTests();
    runAllMotorCommanderTests();
        
    UNITY_END();
    
}

void loop() {
    //Do nothing here.
}
