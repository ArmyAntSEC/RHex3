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
    
    unsigned long int timeToMove = 1000;  
    
    //First spin up the motor
    driver.setMotorPWM(128);     
        
    sched.delayWithScheduler(timeToMove);
            
    //Reset the position and then move at constant speed.
    encoder->forceHomed();  
    sched.delayWithScheduler(timeToMove);
    
    long int endPos = encoder->getPosition().getSerialPosition();                        
    long int endPosTarget = 3250; //Aproximately how far it should move in 1 second        
    TEST_ASSERT_INT_WITHIN( 300, endPosTarget, endPos );                             
    
}

void testSimpleHoming() {    
    unsigned long int maxTimeToMove = 5000;  
    unsigned long int endTime = millis() + maxTimeToMove;    

    driver.setMotorPWM(128);    
    encoder->unHome(); //And reset the homing flag.    

    boolean firstRound = true;
    while ( millis() < endTime ) {   
        if ( encoder->isHomed() ) {
            if ( firstRound ) {
                unsigned long int endPos = encoder->getPosAtLastHome();
                TEST_ASSERT_LESS_THAN( 1796, endPos ); //Make sure we home within one rotation            
                encoder->unHome(); //Then we unhome the encoder to keep moving
                firstRound = false;    
                //Log << "First homing" << endl;        
            } else {
                driver.setMotorPWM(0);
                unsigned long int endPos = encoder->getPosAtLastHome();
                TEST_ASSERT_INT_WITHIN( 10, 1796, endPos ); //Make sure we have gone exactly 1 round            
                //Log << "Second homing" << endl;
                return;
            }
        }
        sched.run();
    }    
    TEST_ASSERT_TRUE( encoder->isHomed() );    
}

void testWrapAroundAndOffset() {
    unsigned long int timeToMove = 1500;  

    driver.setMotorPWM(128);    
        
    sched.delayWithScheduler( timeToMove );

    RotationPositionWithLaps pos = encoder->getPosition();
    TEST_ASSERT_LESS_THAN( 3593, pos.getClickPosition() );    
    TEST_ASSERT_EQUAL( 2, pos.getLaps() );    
}

void testEncoderForStandingStill()
{
    unsigned long int timeToMove = 1500;  
    driver.setMotorPWM(0);            
    sched.delayWithScheduler( timeToMove );    
    
    sched.delayWithScheduler( 10 );    

    int speed = encoder->getSpeedCPS();    
    
    int speedFiltered = encoder->getSpeedCPSFiltered();    
    TEST_ASSERT_INT_WITHIN( 10, 0, speedFiltered );                    
}

void testSimpleMoveWithSpeed() {        

    unsigned long int timeToMove = 1000;  

    //First spin up the motor    
    driver.setMotorPWM(128);     
    sched.delayWithScheduler( timeToMove );

    //Measure the speed
    int speed = encoder->getSpeedCPSFiltered();    
    TEST_ASSERT_INT_WITHIN( 150, 3252, speed );                    

    //Compute the distance to move
    timeToMove = 1000;          
    encoder->forceHomed();        
    sched.delayWithScheduler( timeToMove );

    long int endPosTarget = (timeToMove*speed)/1000;    
    long int endPos = encoder->getPosition().getSerialPosition();                
    TEST_ASSERT_INT_WITHIN( 100, endPosTarget, endPos );                
}

void doTestMoveWithPredictedSpeed( unsigned int power );

void testMoveWithPredictedSpeedPower32()
{
    doTestMoveWithPredictedSpeed(32);
}

void testMoveWithPredictedSpeedPower64()
{
    doTestMoveWithPredictedSpeed(64);
}

void testMoveWithPredictedSpeedPower128()
{
    doTestMoveWithPredictedSpeed(128);
}

void testMoveWithPredictedSpeedPower255()
{
    doTestMoveWithPredictedSpeed(255);
}


void doTestMoveWithPredictedSpeed( unsigned int power ) {        

    unsigned long int motorSettlingTime = 1000;      
    
    SpeedToPowerConverterProduction converter;
    
    converter.initFromEEPROM();    
    
    unsigned int predictedSpeed = converter.GetFreeSpeedForPower( power );    

    //Spin up the motor
    driver.setMotorPWM(255);     
    sched.delayWithScheduler( motorSettlingTime );        

    //Set the speed, settle and measure
    driver.setMotorPWM(power);     
    sched.delayWithScheduler( motorSettlingTime );        
    int measuredSpeed = encoder->getSpeedCPSFiltered();      
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