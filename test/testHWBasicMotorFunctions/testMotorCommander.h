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

    int speedComputed = commander.computeTargetSpeed(timeLeft, clicksLeft);
    TEST_ASSERT_INT_WITHIN(2, 2000, speedComputed);
}

void testComputeTargetSpeedNegativeTime(void)
{
    int timeLeft = -100;
    int clicksLeft = 200;
    MotorSpeedCommander commander;

    int speedComputed = commander.computeTargetSpeed(timeLeft, clicksLeft);
    TEST_ASSERT_INT_WITHIN(2, commander.getMaxSpeed(), speedComputed);
}

void testComputeTargetSpeedTooFast(void)
{
    int timeLeft = 100;
    int clicksLeft = 5000;
    MotorSpeedCommander commander;

    int speedComputed = commander.computeTargetSpeed(timeLeft, clicksLeft);
    TEST_ASSERT_INT_WITHIN(2, commander.getMaxSpeed(), speedComputed);
}

void testComputeTargetSpeedPastPoint(void)
{
    int timeLeft = 100;
    int clicksLeft = -200;
    MotorSpeedCommander commander;

    int speedComputed = commander.computeTargetSpeed(timeLeft, clicksLeft);
    TEST_ASSERT_INT_WITHIN(2, 1, speedComputed);
}

void testSimpleMoveToAPositionAtTime()
{
    unsigned long int timeToMove = 500;
    unsigned long int posToMoveTo = 1500;

    regulator.init();
    encoder->forceHomed();

    unsigned long int endTime = millis() + timeToMove;
    commander.init(endTime, posToMoveTo);

    //Log << "Phase 1: Goal pos: " << posToMoveTo << " at time " << endTime << endl;
    boolean hasArrived = false;
    while (!hasArrived)
    {
        if (millis() > endTime + 5000)
        {
            TEST_FAIL_MESSAGE("Commander never arrived");
            break;
        }
        else if (commander.hasArrived())
        {
            unsigned long int pos = encoder->getPosition().getClickPosition();
            TEST_ASSERT_INT_WITHIN(100, posToMoveTo, pos);
            TEST_ASSERT_INT_WITHIN(100, endTime, millis());
            hasArrived = true;
        }
        sched.run();
    }

    //Now move almost one more complete round
    timeToMove = 1000;
    posToMoveTo = 1000;
    endTime = millis() + timeToMove;

    commander.init(endTime, posToMoveTo);

    //Log << "Phase 2: Goal pos: " << posToMoveTo << " at time " << endTime << endl;
    hasArrived = false;
    while (!hasArrived)
    {
        if (millis() > endTime + 5000)
        {
            TEST_FAIL_MESSAGE("Commander never arrived");
            break;
        }
        if (commander.hasArrived())
        {
            unsigned long int pos = encoder->getPosition().getClickPosition();
            TEST_ASSERT_INT_WITHIN(100, posToMoveTo, pos);
            TEST_ASSERT_INT_WITHIN(100, endTime, millis());
            hasArrived = true;
        }
        sched.run();
    } 
}

void runAllMotorCommanderTests()
{
    RUN_TEST(testComputeTargetSpeedWithinRange);
    RUN_TEST(testComputeTargetSpeedNegativeTime);
    RUN_TEST(testComputeTargetSpeedTooFast);
    RUN_TEST(testComputeTargetSpeedPastPoint);

    sched.delayWithScheduler(500);
    RUN_TEST(testSimpleMoveToAPositionAtTime);
}

#endif