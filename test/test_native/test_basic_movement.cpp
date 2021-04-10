#include <unity.h>
#include <HomingEncoder.h>
#include "MotorDriver.h"
#include "RecurringEncoderWrapper.h"
#include <TaskScheduler.h>
#include "RecurringTaskGroup.h"

HomingEncoder encoder;
EncoderWrapperComputeSpeedTask encoderWrapperComputeSpeed ( &encoder );
RecurringEncoderWrapperHoming<0> encoderWrapperHoming ( &encoder );

MotorDriver driver;

TaskScheduler sched;
RecurringTaskGroup<16> recurring10ms( 10 );

unsigned long int timeToMove = 1000;  
unsigned long int endTime = 0;
unsigned long int startPos = 0;


// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

#include <Arduino.h>

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);        
    UNITY_BEGIN();

    TEST_FAIL_MESSAGE("This is a test" );

    UNITY_END();

    recurring10ms.add( &encoderWrapperComputeSpeed );    
    recurring10ms.init( millis() );  
    sched.add( &recurring10ms );    
}

void loop() {
    if ( endTime == 0 ) {
        endTime = millis() + timeToMove;
        startPos = encoder.getPosComp();
        driver.setMotorPWM(128);    
    }
    if ( endTime < millis() ) {
        unsigned long int endPos = encoder.getPosComp();
        unsigned long int clicksMoved = endPos - startPos;
        TEST_ASSERT_INT_WITHIN( 1000, 5000, clicksMoved ); 
    }
    sched.run();
}