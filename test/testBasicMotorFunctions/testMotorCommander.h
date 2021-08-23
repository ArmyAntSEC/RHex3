#ifndef _TESTMOTORCOMMANDER_H_
#define _TESTMOTORCOMMANDER_H_

#include <Arduino.h>
#include <unity.h>
#include <TaskScheduler.h>
#include "RecurringTaskGroup.h"
#include <configureOneLeg.h>

#include "testBasicMotorFunctions.h"

void testComputeTargetSpeedWithinRange(void)
{
    int timeLeft = 100;
    int clicksLeft = 200;    
    MotorSpeedCommander commander;

    int speedComputed = commander.computeTargetSpeed( timeLeft, clicksLeft );
    TEST_ASSERT_INT_WITHIN ( 2, 2000, speedComputed );
}

void testComputeTargetSpeedNegativeTime(void)
{
    int timeLeft = -100;
    int clicksLeft = 200;    
    MotorSpeedCommander commander;

    int speedComputed = commander.computeTargetSpeed( timeLeft, clicksLeft );
    TEST_ASSERT_INT_WITHIN ( 2, commander.getMaxSpeed(), speedComputed );
}

void testComputeTargetSpeedTooFast(void)
{
    int timeLeft = 100;
    int clicksLeft = 5000;    
    MotorSpeedCommander commander;

    int speedComputed = commander.computeTargetSpeed( timeLeft, clicksLeft );        
    TEST_ASSERT_INT_WITHIN ( 2, commander.getMaxSpeed(), speedComputed );
}

void testComputeTargetSpeedPastPoint(void)
{
    int timeLeft = 100;
    int clicksLeft = -200;    
    MotorSpeedCommander commander;

    int speedComputed = commander.computeTargetSpeed( timeLeft, clicksLeft );        
    TEST_ASSERT_INT_WITHIN ( 2, 1, speedComputed );
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

void runAllMotorCommanderTests()
{
    RUN_TEST(testComputeTargetSpeedWithinRange);
    RUN_TEST(testComputeTargetSpeedNegativeTime);
    RUN_TEST(testComputeTargetSpeedTooFast);
    RUN_TEST(testComputeTargetSpeedPastPoint);

    sched.delayWithScheduler(500);        
    //RUN_TEST(testSimpleMoveToAPositionAtTime);    
}

#endif