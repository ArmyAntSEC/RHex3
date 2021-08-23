#ifndef _TESTMOTORREGULATOR_H_
#define _TESTMOTORREGULATOR_H_

#include <Arduino.h>
#include <unity.h>
#include <TaskScheduler.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>

#include "testBasicMotorFunctions.h"

void testSimpleMoveAtConstantSpeed( unsigned int speedToMoveAt) {    

    unsigned long int timeToMove = 2000;  
    unsigned long int timeToSettle = 1000;
    unsigned long int endTime = 0;    

    
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
    unsigned long int nextRun = millis() + 10;
    while ( millis() < endTime ) {                                    
        if ( millis() > nextRun ) {
            nextRun+=10;
            unsigned long int speed = encoder.getSpeedCPS();                        
            long int speedRel = speed - speedToMoveAt;
            
            //Compute STD        
            n++;
            S += speedRel;
            S2 += speedRel*speedRel;
            //Log << "speedRel: " << speedRel << " n: " << n << " S: " << S << " S2: " << S2 << endl;
        }
    }

    unsigned long int speed = encoder.getSpeedCPS();                                    
    //Log << "Speed: " << speed << " SetPoint: " << speedToMoveAt << endl;
    TEST_ASSERT_INT_WITHIN( 200, speedToMoveAt, speed );            
    
    float stdDev = sqrt( (n*S2-S*S)/(n*(n-1)) );
    //Log << "Standard deviation: " << stdDev << endl;
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
    unsigned long int timeToSettle = 100;      
    int speedToMoveAt = 3000;
    
    regulator.setSetPoint( speedToMoveAt );
    regulator.init();                
    //Run to settle the speed.
    unsigned long int endTime = millis() + timeToSettle;
    while ( millis() < endTime ) {
        sched.run();        
    }
    TEST_ASSERT_GREATER_THAN_INT( 0.9*speedToMoveAt, encoder.getSpeedCPS() );            

    regulator.setSetPoint( speedToMoveAt/2 );
    regulator.doHardBreak();
    
    endTime = millis() + timeToSettle;
    while ( millis() < endTime ) {
        sched.run();                
    }
    TEST_ASSERT_LESS_THAN_INT( 1.1*speedToMoveAt/2, encoder.getSpeedCPS() );

}

void testSimpleMoveToAPositionAtTime() {    
    unsigned long int timeToMove = 500;      
    unsigned long int posToMoveTo = 1500;
        
    regulator.init();    
    unsigned long int endTime = millis() + timeToMove;
    commander.init( endTime, posToMoveTo );  

    
    Log << millis() << " Goal pos: " << posToMoveTo << " at time " << endTime << endl;
    boolean hasArrived = false;
    while ( !hasArrived ) {
        if ( commander.hasArrived() || millis() > endTime + 5000 ) {
            unsigned long int pos = encoder.getPosComp();                
            TEST_ASSERT_INT_WITHIN( 100, posToMoveTo, pos );
            TEST_ASSERT_INT_WITHIN( 100, endTime, millis() );
            hasArrived = true;
        }
        sched.run();
    }
    
    //Now move almost one more complete round
    timeToMove = 1000;
    posToMoveTo = 1000;
    endTime = millis() + timeToMove;
    
    commander.init( endTime, posToMoveTo );  
    
    Log << millis() << " Goal pos: " << posToMoveTo << " at time " << endTime << endl;
    hasArrived = false;
    while ( !hasArrived ) {
        if ( commander.hasArrived() || millis() > endTime + 5000 ) {
            unsigned long int pos = encoder.getPosComp();                
            TEST_ASSERT_INT_WITHIN( 100, posToMoveTo, pos );
            TEST_ASSERT_INT_WITHIN( 100, endTime, millis() );
            hasArrived = true;
        }
        sched.run();
    }    
    TEST_ASSERT_TRUE( hasArrived );
}

#endif