#ifndef _TESTBASICMOVEMENT_H_
#define _TESTBASICMOVEMENT_H_

#include <Arduino.h>
#include <unity.h>
#include <TaskScheduler.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>
#include <SpeedToPowerConverter.h>

#include "testBasicMotorFunctions.h"

void testSimpleMove() {        
    
    unsigned long int16_t timeToMove = 1000;  
    
    //First spin up the motor
    driver.setMotorPWM(128);     
        
    sched.delayWithScheduler(timeToMove);
            
    //Reset the position and then move at constant speed.
    encoder->forceHomed();  
    sched.delayWithScheduler(timeToMove);
    
    long int16_t endPos = encoder->getPosition().getSerialPosition();                        
    long int16_t endPosTarget = 3000; //Aproximately how far it should move in 1 second        
    TEST_ASSERT_INT_WITHIN( 1000, endPosTarget, endPos ); //rather generous tolerances as we are just testing basic functionality here.
    
}

void testSimpleHoming() {    
    unsigned long int16_t maxTimeToMove = 5000;  
    unsigned long int16_t endTime = millis() + maxTimeToMove;    

    driver.setMotorPWM(128);    
    encoder->unHome(); //And reset the homing flag.    

    boolean firstRound = true;
    while ( millis() < endTime ) {   
        if ( encoder->isHomed() ) {
            if ( firstRound ) {
                unsigned long int16_t endPos = encoder->getPosAtLastHome().getClickPosition();
                TEST_ASSERT_LESS_THAN( 1796, endPos ); //Make sure we home within one rotation            
                encoder->unHome(); //Then we unhome the encoder to keep moving
                firstRound = false;                    
            } else {
                driver.setMotorPWM(0);
                unsigned long int16_t endPos = encoder->getPosAtLastHome().getClickPosition();                
                TEST_ASSERT_INT_WITHIN( 10, 1796, endPos ); //Make sure we have gone exactly 1 round                                            
                return;
            }
        }
        sched.run();
    }    
    TEST_ASSERT_TRUE( encoder->isHomed() );    
}

void testWrapAroundAndOffset() {
    unsigned long int16_t timeToMove = 1500;  

    driver.setMotorPWM(128);    
        
    sched.delayWithScheduler( timeToMove );

    RotationPositionWithLaps pos = encoder->getPosition();
    TEST_ASSERT_LESS_THAN( 3593, pos.getClickPosition() );    
    TEST_ASSERT_EQUAL( 2, pos.getLaps() );    
}

void testEncoderForStandingStill()
{
    unsigned long int16_t timeToMove = 1500;  
    driver.setMotorPWM(0);            
    sched.delayWithScheduler( timeToMove );    
    
    sched.delayWithScheduler( 10 );    

    int16_t speed = encoder->getSpeedCPS();    
    
    int16_t speedFiltered = encoder->getSpeedCPSFiltered();    
    TEST_ASSERT_INT_WITHIN( 10, 0, speedFiltered );                    
}

void testSimpleMoveWithSpeed() {        
    TEST_IGNORE();

    unsigned long int16_t timeToMove = 1000;  

    //First spin up the motor    
    driver.setMotorPWM(128);     
    sched.delayWithScheduler( timeToMove );

    //Measure the speed
    int16_t speed = encoder->getSpeedCPSFiltered();    
    TEST_ASSERT_INT_WITHIN( 600, 3252, speed ); //very loose tolerances as we are just sanity checking the system here.

    //Compute the distance to move
    timeToMove = 1000;          
    encoder->forceHomed();        
    sched.delayWithScheduler( timeToMove );

    long int16_t endPosTarget = (timeToMove*speed)/1000;    
    long int16_t endPos = encoder->getPosition().getSerialPosition();                
    TEST_ASSERT_INT_WITHIN( 200, endPosTarget, endPos );
}

void doTestMoveWithPredictedSpeed( unsigned int16_t power );

void testMoveWithPredictedSpeedPower32()
{
    doTestMoveWithPredictedSpeed(32);
}

void testMoveWithPredictedSpeedPower64()
{
    TEST_IGNORE();
    doTestMoveWithPredictedSpeed(64);
}

void testMoveWithPredictedSpeedPower128()
{
    TEST_IGNORE();
    doTestMoveWithPredictedSpeed(128);
}

void testMoveWithPredictedSpeedPower255()
{
    TEST_IGNORE();
    doTestMoveWithPredictedSpeed(255);
}


void doTestMoveWithPredictedSpeed( unsigned int16_t power ) {        

    unsigned long int16_t motorSettlingTime = 1000;      
    
    SpeedToPowerConverterProduction converter;
    
    converter.initFromEEPROM();    
    
    unsigned int16_t predictedSpeed = converter.GetFreeSpeedForPower( power );    

    //Spin up the motor
    driver.setMotorPWM(255);     
    sched.delayWithScheduler( motorSettlingTime );        

    //Set the speed, settle and measure
    driver.setMotorPWM(power);     
    sched.delayWithScheduler( motorSettlingTime );        
    int16_t measuredSpeed = encoder->getSpeedCPSFiltered();      
    TEST_ASSERT_INT_WITHIN( 150, predictedSpeed, measuredSpeed );                    
}
#endif

void runAllBasicMovementTests()
{
    RUN_TEST(testSimpleMove);  
    
    sched.delayWithScheduler(500);
    RUN_TEST(testSimpleHoming);        
    
    sched.delayWithScheduler(500);
    RUN_TEST(testWrapAroundAndOffset); 
    
    sched.delayWithScheduler(500);
    RUN_TEST(testEncoderForStandingStill);
    
    sched.delayWithScheduler(500);
    RUN_TEST(testSimpleMoveWithSpeed);  
    
    sched.delayWithScheduler(500);
    RUN_TEST(testMoveWithPredictedSpeedPower32);
    
    sched.delayWithScheduler(500);
    RUN_TEST(testMoveWithPredictedSpeedPower64);
    
    sched.delayWithScheduler(500);
    RUN_TEST(testMoveWithPredictedSpeedPower128);
    
    sched.delayWithScheduler(500);
    RUN_TEST(testMoveWithPredictedSpeedPower255);

}