#include <Arduino.h>
#include <unity.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>

#include "testBasicMotorFunctions.h"
#include "testBasicMovement.h"
#include "testMotorRegulator.h"


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
    RUN_TEST(testWrapAroundLogic);  
    RUN_TEST(testPositiveSubtraction);    
    delay(500);
    RUN_TEST(testSimpleMove);  
    delay(500);    
    RUN_TEST(testSimpleHoming);        
    delay(500);
    RUN_TEST(testWrapAroundAndOffset); 
    delay(500);
    RUN_TEST(testSimpleMoveWithSpeed);  
    delay(500);    
    RUN_TEST(testMoveWithPredictedSpeed);
    
    /*
    delay(500);
    RUN_TEST(testSimpleMoveAtConstantSpeed7000);    
    delay(500);    
    RUN_TEST(testSimpleMoveAtConstantSpeed4000);
    delay(500);
    RUN_TEST(testSimpleMoveAtConstantSpeed2000);
    delay(500);    
    RUN_TEST(testSimpleMoveAtConstantSpeed1000);    
    delay(500);         
    RUN_TEST(testSimpleMoveToAPositionAtTime);
    delay(500);    
    RUN_TEST(testRegulatorHardBreak);
    */
    UNITY_END();
}

void loop() {
    //Do nothing here.
}
