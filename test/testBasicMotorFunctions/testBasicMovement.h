#ifndef _TESTBASICMOVEMENT_H_
#define _TESTBASICMOVEMENT_H_

#include <Arduino.h>
#include <unity.h>
#include <TaskScheduler.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>

#include "testBasicMotorFunctions.h"

void testWrapAroundLogic() {
    double lapRemainder = (double)encoder.clicksPerRevolution - 1795;

    encoder.state.raw_position = 0;
    encoder.state.position_remainder = 0;
    encoder.state.laps = 0;
    encoder.handleOverflow();
    TEST_ASSERT_EQUAL( 0, encoder.getRawPos() );    
    TEST_ASSERT_EQUAL( 0, encoder.getLaps() );
    
    encoder.state.raw_position = 1796;
    encoder.state.position_remainder = 0;    
    encoder.state.laps = 0;
    encoder.handleOverflow ( );
    TEST_ASSERT_EQUAL( 0, encoder.getRawPos() );
    TEST_ASSERT_DOUBLE_WITHIN(5e-4, 1-lapRemainder, (double)encoder.state.position_remainder );
    TEST_ASSERT_EQUAL( 1, encoder.getLaps() );

    encoder.state.raw_position = 1798;
    encoder.state.position_remainder = 0;    
    encoder.state.laps = 0;
    encoder.handleOverflow ( );
    TEST_ASSERT_EQUAL( 2, encoder.getRawPos() );
    TEST_ASSERT_DOUBLE_WITHIN(5e-4, 1-lapRemainder, (double)encoder.state.position_remainder );
    TEST_ASSERT_EQUAL( 1, encoder.getLaps() );

    encoder.state.raw_position = 1796*2;
    encoder.state.position_remainder = 0;    
    encoder.state.laps = 0;
    encoder.handleOverflow ();
    encoder.handleOverflow ();
    TEST_ASSERT_EQUAL( 0, encoder.getRawPos() );
    TEST_ASSERT_DOUBLE_WITHIN(2*5e-4, 2*(1-lapRemainder), (double)encoder.state.position_remainder );
    TEST_ASSERT_EQUAL( 2, encoder.getLaps() );

}

void testPositiveSubtraction() {
    TEST_ASSERT_EQUAL( 10, encoder.positionPositiveDifference( 10, 0 ) );
    TEST_ASSERT_EQUAL( 1795-10, encoder.positionPositiveDifference( 0, 10 ) );
}

void testSimpleMove() {        
    
    unsigned long int timeToMove = 1000;  
    unsigned long int startTime = millis();
    unsigned long int endTime =  startTime + timeToMove;        

    //First spin up the motor
    driver.setMotorPWM(128);     
    delay(timeToMove);
            
    //Reset the position and then move at constant speed.
    encoder.forceHomed();  
    delay(timeToMove);

    long int endPos = encoder.getRawPos();                
    long int endPosTarget = 2700; //Aproximately how far it should move in 1 second
    TEST_ASSERT_INT_WITHIN( 300, endPosTarget, endPos );                             
}

void testSimpleHoming() {
    unsigned long int maxTimeToMove = 5000;  
    unsigned long int endTime = millis() + maxTimeToMove;    

    driver.setMotorPWM(128);    
    encoder.unHome(); //And reset the homing flag.    

    boolean firstRound = true;
    while ( millis() < endTime ) {        
        if ( encoder.isHomed() ) {
            if ( firstRound ) {
                unsigned long int endPos = encoder.getPosAtLastHome();
                TEST_ASSERT_LESS_THAN( 1796, endPos ); //Make sure we home within one rotation            
                encoder.unHome(); //Then we unhome the encoder to keep moving
                firstRound = false;    
                Log << "First homing" << endl;        
            } else {
                driver.setMotorPWM(0);
                unsigned long int endPos = encoder.getPosAtLastHome();
                TEST_ASSERT_INT_WITHIN( 10, 1796, endPos ); //Make sure we have gone exactly 1 round            
                Log << "Second homing" << endl;
                return;
            }
        }
        sched.run();
    }    
    TEST_ASSERT_TRUE( encoder.isHomed() );    
}

void testWrapAroundAndOffset() {
    unsigned long int maxTimeToMove = 1500;  
    unsigned int startTime = millis();
    unsigned long int endTime = startTime + maxTimeToMove;        

    driver.setMotorPWM(128);    
        
    while ( millis() < endTime ) {                
        sched.run();
    }    
    TEST_ASSERT_LESS_THAN( 3593, encoder.getPosComp() );    
    TEST_ASSERT_EQUAL( 2, encoder.getLaps() );    
}


void testSimpleMoveWithSpeed() {        

    unsigned long int timeToMove = 1000;  
    unsigned long int startTime = millis();
    unsigned long int endTime =  startTime + timeToMove;        

    //First spin up the motor    
    driver.setMotorPWM(128);     
    while ( millis() < endTime ) {
        sched.run();
    }
    
    //Measure the speed
    int speed = encoder.getSpeedCPS();    
    TEST_ASSERT_INT_WITHIN( 150, 2800, speed );                    

    //Compute the distance to move
    timeToMove = 1000;  
    startTime = endTime; //We continue where we left off.
    endTime =  startTime + timeToMove;        
        
    encoder.forceHomed();    
    long int endPosTarget = (timeToMove*speed)/1000;
    int lapsTarget = endPosTarget / HomingEncoder::clicksPerRevolution.getInteger();  //Approximate.
    long int endPosTargetWrapped = endPosTarget % HomingEncoder::clicksPerRevolution.getInteger();  //Approximate.
    
    while ( millis() < endTime ) {        
        sched.run();
    }    
    long int endPos = encoder.getRawPos();            
    long int laps = encoder.getLaps();    
    TEST_ASSERT_INT_WITHIN( 100, endPosTargetWrapped, endPos );            
    TEST_ASSERT_EQUAL( lapsTarget, laps );            
}

#endif