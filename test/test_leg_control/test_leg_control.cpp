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
    encoderWrapperHoming.init( millis() ); //Start the homing
    regulator.setSetPoint(0);
    regulator.stop();
    driver.setMotorPWM(0);    
}

void tearDown(void) {
    driver.setMotorPWM(0);
    leg.stop();
}


void testStepComputation() {    
    leg.init( 1500, 2000, 1000, 1000, LegController::AfterStep );
    
    int* posSequence1 = leg.getPosSequence(0);
    int time1 = *(posSequence1);
    int pos1 = *(posSequence1+1);
    
    int* posSequence2 = leg.getPosSequence(1);
    int time2 = *(posSequence2);
    int pos2 = *(posSequence2+1);
    
    TEST_ASSERT_EQUAL_INT ( 1500+1000, time1 );
    TEST_ASSERT_EQUAL_INT ( 3592-500, pos1 );
    TEST_ASSERT_EQUAL_INT ( 1500+2000, time2 );
    TEST_ASSERT_EQUAL_INT ( 500, pos2 );

    leg.init( 1700, 1800, 1100, 800, LegController::BeforeStep );
    
    posSequence1 = leg.getPosSequence(0);
    time1 = *(posSequence1);
    pos1 = *(posSequence1+1);
    
    posSequence2 = leg.getPosSequence(1);
    time2 = *(posSequence2);
    pos2 = *(posSequence2+1);
    
    TEST_ASSERT_EQUAL_INT ( 1700+1100, time1 );
    TEST_ASSERT_EQUAL_INT ( 400, pos1 );
    TEST_ASSERT_EQUAL_INT ( 1700+1800, time2 );
    TEST_ASSERT_EQUAL_INT ( 3592-400, pos2 );

}

void testStepComputationWithMoveAfterStep() {
    
    int stepTime = 1000;
    int slowTime = 300;
    int slowAngle = 1000;

    leg.init( millis(), stepTime, slowTime, slowAngle, LegController::AfterStep );    

    unsigned long int fastTimestamp = millis() + stepTime - slowTime;
    unsigned long int slowTimestamp = millis() + stepTime;
    
    while ( millis() < fastTimestamp )
    {        
        sched.run();
    }

    unsigned long int pos = encoder.getPosComp();
    unsigned long int expectedPos = 3592-slowAngle/2;        
    TEST_ASSERT_INT_WITHIN( 40, expectedPos, pos );    
    TEST_ASSERT_EQUAL ( 0, encoder.getLaps() );

    while ( millis() < slowTimestamp )
    {        
        sched.run();
    }

    pos = encoder.getPosComp();
    expectedPos = slowAngle/2;    
    TEST_ASSERT_INT_WITHIN( 40, expectedPos, pos );    
    TEST_ASSERT_EQUAL ( 1, encoder.getLaps() );
}

void testStepComputationWithMoveAfterStepTwice() {
    
    int stepTime = 2000;
    int slowTime = 1000;
    int slowAngle = 1000;

    //Take a first step.
    leg.init( millis(), stepTime, slowTime, slowAngle, LegController::AfterStep );    

    unsigned long int endTime = millis() + stepTime;    
    
    while ( millis() < endTime )
    {        
        sched.run();
    }

    unsigned long int pos = encoder.getPosComp();
    unsigned long int expectedPos = slowAngle/2;        
    TEST_ASSERT_INT_WITHIN( 40, expectedPos, pos );    
    TEST_ASSERT_EQUAL ( 1, encoder.getLaps() );

    //Take a second step.
    leg.init( millis(), stepTime, slowTime, slowAngle, LegController::AfterStep );    

    endTime = millis() + stepTime;    
    
    while ( millis() < endTime )
    {        
        sched.run();
    }

    pos = encoder.getPosComp();
    expectedPos = slowAngle/2;        
    TEST_ASSERT_INT_WITHIN( 40, expectedPos, pos );    
    TEST_ASSERT_EQUAL ( 2, encoder.getLaps() );
}

void testStepComputationWithMoveBeforeStep() {
    
    int stepTime = 2000;
    int slowTime = 1000;
    int slowAngle = 1000;

    leg.init( millis(), stepTime, slowTime, slowAngle, LegController::BeforeStep );    

    unsigned long int slowTimestamp = millis() + slowTime;
    unsigned long int fastTimestamp = millis() + stepTime;
    //Log << "Slow time: " << slowTimestamp << " Fast Time: " << fastTimestamp << endl;

    unsigned long int nextTime = millis() + 100;
    //Log << millis() << ": Slow started: " << millis() << endl;
    while ( millis() < slowTimestamp )
    {        
        sched.run();
        if ( millis() > nextTime ) {
            nextTime += 100;
            //Log << millis() << ": Slow: " << encoder.getPosComp() << endl;
        }
    }
    //Log << millis() << ": Slow ended: " << millis() << endl;

    unsigned long int pos = encoder.getPosComp();
    unsigned long int expectedPos = slowAngle/2;        
    //TEST_ASSERT_INT_WITHIN( 20, expectedPos, pos );    
    //TEST_ASSERT_EQUAL ( 0, encoder.getLaps() );

    
    while ( millis() < fastTimestamp )
    {        
        sched.run();
        if ( millis() > nextTime ) {
            nextTime += 100;
            //Log << millis() << ": Fast: " << encoder.getPosComp() << endl;
        }
    }
    //Log << millis() << ": Fast ended: " << millis() << endl;

    pos = encoder.getPosComp();
    expectedPos = 3592 - slowAngle/2;    
    TEST_ASSERT_INT_WITHIN( 20, expectedPos, pos );    
    TEST_ASSERT_EQUAL ( 0, encoder.getLaps() );

    while ( millis() < fastTimestamp + 500 )
    {        
        sched.run();
    }

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
    RUN_TEST(testStepComputationWithMoveAfterStep);    
    delay(500);
    RUN_TEST(testStepComputationWithMoveBeforeStep);
    delay(500);    
    RUN_TEST(testStepComputationWithMoveAfterStepTwice);        
    UNITY_END();
}

void loop() {
    //Do nothing here.
}
