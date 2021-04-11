#include <Arduino.h>
#include <unity.h>
#include <TaskScheduler.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>


TaskScheduler sched;
RecurringTaskGroup<16> recurring10ms( 10 );


// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }


void testSimpleMove() {
    unsigned long int timeToMove = 1000;  
    unsigned long int endTime = 0;
    unsigned long int startPos = 0;

    while ( true ) {
        if ( endTime == 0 ) {
            endTime = millis() + timeToMove;
            startPos = encoder.getPosComp();
            driver.setMotorPWM(128);            
        }
        if ( endTime < millis() ) {
            unsigned long int endPos = encoder.getPosComp();
            unsigned long int clicksMoved = endPos - startPos;
            driver.setMotorPWM(0);             
            TEST_ASSERT_INT_WITHIN( 200, 6000, clicksMoved );            
            return;
        }
        sched.run();
    }
}

void testSimpleMoveAtConstantSpeed( unsigned int speedToMoveAt) {
    unsigned long int timeToMove = 2000;  
    unsigned long int endTime = 0;    

    while ( true ) {
        if ( endTime == 0 ) {            
            endTime = millis() + timeToMove;
            regulator.setSetPoint( speedToMoveAt );
            regulator.start();            
        }
        if ( endTime < millis() ) {
            unsigned long int speed = encoder.getSpeedCPMS();
            driver.setMotorPWM(0); 
            regulator.stop();
            TEST_ASSERT_INT_WITHIN( 200, speedToMoveAt, speed );            
            return;
        }
        sched.run();
    }
}

void testSimpleMoveAtConstantSpeed7000() {
    testSimpleMoveAtConstantSpeed(7000);  
}

void testSimpleMoveAtConstantSpeed4000() {
    testSimpleMoveAtConstantSpeed(4000);  
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    initOneLeg();

    regulator.stop(); //Don't start the regulator from the get go.

    recurring10ms.add( &encoderWrapperComputeSpeed );    
    recurring10ms.add( &regulator );
    recurring10ms.init( millis() );  
    sched.add( &recurring10ms ); 

    UNITY_BEGIN();
    RUN_TEST(testSimpleMove);  
    delay(500);
    RUN_TEST(testSimpleMoveAtConstantSpeed7000);
    delay(500);
    RUN_TEST(testSimpleMoveAtConstantSpeed4000);
    UNITY_END();
}

void loop() {
    //Do nothing here.
}
