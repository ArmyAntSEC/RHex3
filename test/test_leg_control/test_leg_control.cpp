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
    
    int* posSequence1 = leg.getPosSequence(0);
    int time1 = *(posSequence1);
    int pos1 = *(posSequence1+1);
    
    int* posSequence2 = leg.getPosSequence(1);
    int time2 = *(posSequence2);
    int pos2 = *(posSequence2+1);
    
    TEST_ASSERT_EQUAL_INT ( 1000, time1 );
    TEST_ASSERT_EQUAL_INT ( 3592-500, pos1 );
    TEST_ASSERT_EQUAL_INT ( 2000, time2 );
    TEST_ASSERT_EQUAL_INT ( 500, pos2 );
}

void testStepComputationWithMove() {
    
    int stepTime = 2000;
    int slowTime = 1000;
    int slowAngle = 1000;

    leg.init( millis(), stepTime, slowTime, slowAngle, LegController::AfterStep );    
    regulator.init();

    unsigned long int cutoffTime = millis() + stepTime + 1000;
        
    boolean checkAfterFast = false;
    unsigned long int fastTimestamp = millis() + stepTime - slowTime/2;
    
    boolean checkAfterSlow = false;
    unsigned long int slowTimestamp = millis() + stepTime + slowTime/2;
        
    while ( millis() < cutoffTime )
    {
        leg.run(millis());

        if ( !checkAfterFast && millis() > fastTimestamp ) {
            unsigned long int pos = encoder.getPosComp();
            unsigned long int expected = 3592-slowAngle/2;
            Log << "Pos: " << pos << " Expected: " << expected << endl;
            TEST_ASSERT_INT_WITHIN( 100, expected, pos );
            checkAfterFast = true; //Only do this once
        }

        if ( !checkAfterSlow && millis() > slowTimestamp ) {
            unsigned long int pos = encoder.getPosComp();
            unsigned long int expected = 3592 + slowAngle/2;
            Log << "Pos: " << pos << " Expected: " << expected << endl;
            TEST_FAIL_MESSAGE( "Need to handle the wrap-around that happens somehow...." );
            TEST_ASSERT_INT_WITHIN( 100, expected, pos );
            checkAfterSlow = true; //Only do this once
        }

        sched.run();
    }

    TEST_ASSERT_TRUE( checkAfterFast );
    TEST_ASSERT_TRUE( checkAfterSlow );
    driver.setMotorPWM(0);

    
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
    recurring10ms.add( &leg );
    recurring10ms.init( millis() );  
    sched.add( &recurring10ms ); 
    sched.add( &encoderWrapperHoming );


    UNITY_BEGIN();
    RUN_TEST(testStepComputation);      
    delay(500);
    RUN_TEST(testStepComputationWithMove);
    UNITY_END();
}

void loop() {
    //Do nothing here.
}
