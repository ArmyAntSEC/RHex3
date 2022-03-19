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
    int16_t time1 = *(posSequence1);
    int16_t pos1 = *(posSequence1+1);
    
    int* posSequence2 = leg.getPosSequence(1);
    int16_t time2 = *(posSequence2);
    int16_t pos2 = *(posSequence2+1);
    
    TEST_ASSERT_EQUAL_int16_t ( 1500+1000, time1 );
    TEST_ASSERT_EQUAL_int16_t ( 3592-500, pos1 );
    TEST_ASSERT_EQUAL_int16_t ( 1500+2000, time2 );
    TEST_ASSERT_EQUAL_int16_t ( 500, pos2 );

    leg.init( 1700, 1800, 1100, 800, LegController::BeforeStep );
    
    posSequence1 = leg.getPosSequence(0);
    time1 = *(posSequence1);
    pos1 = *(posSequence1+1);
    
    posSequence2 = leg.getPosSequence(1);
    time2 = *(posSequence2);
    pos2 = *(posSequence2+1);
    
    TEST_ASSERT_EQUAL_int16_t ( 1700+1100, time1 );
    TEST_ASSERT_EQUAL_int16_t ( 400, pos1 );
    TEST_ASSERT_EQUAL_int16_t ( 1700+1800, time2 );
    TEST_ASSERT_EQUAL_int16_t ( 3592-400, pos2 );

}

void testStepMoveAfterStep() {
    
    int16_t stepTime = 1000;
    int16_t slowTime = stepTime/2;
    int16_t slowAngle = 1000;

    leg.init( millis(), stepTime, slowTime, slowAngle, LegController::AfterStep );    

    int32_t int16_t fastTimestamp = millis() + stepTime - slowTime;
    int32_t int16_t slowTimestamp = millis() + stepTime;
    
    while ( millis() < fastTimestamp )
    {        
        sched.run();
    }

    int32_t int16_t pos = encoder.getPosComp();
    int32_t int16_t expectedPos = 3592-slowAngle/2;        
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

void testStepMoveAfterStepTwice() {
    
    int16_t stepTime = 2000;
    int16_t slowTime = 1000;
    int16_t slowAngle = 1000;

    //Take a first step.
    leg.init( millis(), stepTime, slowTime, slowAngle, LegController::AfterStep );    

    int32_t int16_t endTime = millis() + stepTime;    
    
    while ( millis() < endTime )
    {        
        sched.run();
    }

    int32_t int16_t pos = encoder.getPosComp();
    int32_t int16_t expectedPos = slowAngle/2;        
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

void testStepMoveBeforeStepTwice() {
    
    int16_t stepTime = 2000;
    int16_t slowTime = 1000;
    int16_t slowAngle = 1000;

    //Take a first step.
    leg.init( millis(), stepTime, slowTime, slowAngle, LegController::BeforeStep );    

    int32_t int16_t endTime = millis() + stepTime;        
    while ( millis() < endTime )
    {        
        sched.run();
    }

    int32_t int16_t pos = encoder.getPosComp();
    int32_t int16_t expectedPos = 3592-slowAngle/2;        
    TEST_ASSERT_INT_WITHIN( 40, expectedPos, pos );    
    TEST_ASSERT_EQUAL ( 0, encoder.getLaps() );

    //Take a second step.
    leg.init( millis(), stepTime, slowTime, slowAngle, LegController::BeforeStep );    

    endTime = millis() + stepTime;        
    while ( millis() < endTime )
    {        
        sched.run();
    }

    pos = encoder.getPosComp();
    expectedPos = 3592-slowAngle/2;        
    TEST_ASSERT_INT_WITHIN( 40, expectedPos, pos );    
    TEST_ASSERT_EQUAL ( 1, encoder.getLaps() );
}

void testStepMoveBeforeStep() {
    
    int16_t stepTime = 2000;
    int16_t slowTime = 1000;
    int16_t slowAngle = 1000;

    leg.init( millis(), stepTime, slowTime, slowAngle, LegController::BeforeStep );    

    int32_t int16_t slowTimestamp = millis() + slowTime;
    int32_t int16_t fastTimestamp = millis() + stepTime;    

    int32_t int16_t nextTime = millis() + 100;

    while ( millis() < slowTimestamp )
    {        
        sched.run();
        if ( millis() > nextTime ) {
            nextTime += 100;
        }
    }    

    int32_t int16_t pos = encoder.getPosComp();
    int32_t int16_t expectedPos = slowAngle/2;        
    //TEST_ASSERT_INT_WITHIN( 20, expectedPos, pos );    
    //TEST_ASSERT_EQUAL ( 0, encoder.getLaps() );

    
    while ( millis() < fastTimestamp )
    {        
        sched.run();
        if ( millis() > nextTime ) {
            nextTime += 100;            
        }
    }

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
    RUN_TEST(testStepMoveAfterStep);            
    delay(500);
    RUN_TEST(testStepMoveBeforeStep);
    delay(500);    
    RUN_TEST(testStepMoveAfterStepTwice);                
    delay(500);    
    RUN_TEST(testStepMoveBeforeStepTwice);                
    UNITY_END();
}

void loop() {
    //Do nothing here.
}
