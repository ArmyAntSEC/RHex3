#include <Arduino.h>
#include <unity.h>
#include <TaskScheduler.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>
#include <LegController.h>

TaskScheduler sched;
RecurringTaskGroup<16> recurring10ms( 10 );


void setUp(void) {
    encoder.forceHomed();
}

void tearDown(void) {
    driver.setMotorPWM(0);
    regulator.stop();    
}


void testStepComputation() {
    LegController leg;
    leg.init( 1500, 2000, 1000, 1000, LegController::AfterStep );
    
    unsigned long int* posSequence1 = leg.getPosSequence(0);
    unsigned long int time1 = *(posSequence1);
    unsigned long int pos1 = *(posSequence1+1);
    
    unsigned long int* posSequence2 = leg.getPosSequence(1);
    unsigned long int time2 = *(posSequence2);
    unsigned long int pos2 = *(posSequence2+1);
    
    TEST_ASSERT_EQUAL_INT ( 2500, time1 );
    TEST_ASSERT_EQUAL_INT ( 3592-500, pos1 );
    TEST_ASSERT_EQUAL_INT ( 3500, time2 );
    TEST_ASSERT_EQUAL_INT ( 500, pos2 );
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
    sched.add( &encoderWrapperHoming );


    UNITY_BEGIN();
    RUN_TEST(testStepComputation);      
    delay(500);
    UNITY_END();
}

void loop() {
    //Do nothing here.
}
