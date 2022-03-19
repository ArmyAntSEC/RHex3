#ifndef _TESTMOTORREGULATOR_H_
#define _TESTMOTORREGULATOR_H_

#include <Arduino.h>
#include <unity.h>
#include <TaskScheduler.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>

#include "testBasicMotorFunctions.h"


void testSimpleMoveAtConstantSpeed( unsigned int16_t speedToMoveAt) {    

    int32_t int16_t timeToMove = 2000;  
    int32_t int16_t timeToSettle = 1000;
    int32_t int16_t endTime = 0;    

    
    regulator.setSetPoint( speedToMoveAt );
    regulator.init();            
    
    //Run to settle the speed.
    endTime = millis() + timeToSettle;
    while ( millis() < endTime ) {
        sched.run();        
    }
    
    //Measure the variance
    float n = 0;
    float S = 0;
    float S2 = 0.0;    

    endTime = millis() + timeToMove;
    int32_t int16_t nextRun = millis() + 10;
    while ( millis() < endTime ) {                                    
        if ( millis() > nextRun ) {
            nextRun+=10;
            int32_t int16_t speed = encoder->getSpeedCPS();                        
            int32_t int16_t speedRel = speed - speedToMoveAt;
            
            //Compute STD        
            n++;
            S += speedRel;
            S2 += speedRel*speedRel;            
        }
    }

    int32_t int16_t speed = encoder->getSpeedCPS();                                    
    TEST_ASSERT_INT_WITHIN( 200, speedToMoveAt, speed );            
    
    float stdDev = sqrt( (n*S2-S*S)/(n*(n-1)) );    
    TEST_ASSERT_FLOAT_WITHIN( 100, 0, stdDev );            

}

void testSimpleMoveAtConstantSpeed3500() {
    testSimpleMoveAtConstantSpeed(3500);  
}

void testSimpleMoveAtConstantSpeed2000() {
    testSimpleMoveAtConstantSpeed(2000);  
}

void testSimpleMoveAtConstantSpeed1000() {
    testSimpleMoveAtConstantSpeed(1000);  
}

void testSimpleMoveAtConstantSpeed500() {
    testSimpleMoveAtConstantSpeed(500);  
}

void testRegulatorHardBreak()
{
    TEST_IGNORE();
    int32_t int16_t timeToSettle = 100;      
    int16_t speedToMoveAt = 3000;
    
    regulator.setSetPoint( speedToMoveAt );
    regulator.init();                
    //Run to settle the speed.
    int32_t int16_t endTime = millis() + timeToSettle;
    while ( millis() < endTime ) {
        sched.run();        
    }
    TEST_ASSERT_GREATER_THAN_INT( 0.9*speedToMoveAt, encoder->getSpeedCPS() );            

    regulator.setSetPoint( speedToMoveAt/2 );
    regulator.useHardBreaks();
    
    endTime = millis() + timeToSettle;
    while ( millis() < endTime ) {
        sched.run();                
    }
    TEST_ASSERT_LESS_THAN_INT( 1.1*speedToMoveAt/2, encoder->getSpeedCPS() );

}

void runAllMotorRegulatorTests()
{       
    
    sched.delayWithScheduler(500);    
    RUN_TEST(testSimpleMoveAtConstantSpeed3500);    
    
    sched.delayWithScheduler(500);    
    RUN_TEST(testSimpleMoveAtConstantSpeed2000);    
    
    sched.delayWithScheduler(500);    
    RUN_TEST(testSimpleMoveAtConstantSpeed1000);    
    
    sched.delayWithScheduler(500);    
    RUN_TEST(testSimpleMoveAtConstantSpeed500);    

    sched.delayWithScheduler(500);    
    RUN_TEST(testRegulatorHardBreak);    
}


#endif