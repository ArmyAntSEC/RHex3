#include <unityEx.h>

#define private public
#include <MotorSpeedCommander.h>

struct SpeedRegulatorMock : public SpeedRegulatorInterface
{
    int16_t setPoint = 1234;
    bool isRunning = false;

    virtual void setSetPoint(int16_t _setPoint)
    {
        setPoint = _setPoint;
    }

    virtual void start()
    {
        isRunning = true;
    }

    virtual void stop()
    {
        isRunning = false;
    }
};

void testSetGoal()
{

    RotationalPosition currentPos(1234);
    SpeedRegulatorMock speedRegulator;
    MotorSpeedCommander commander(&currentPos, &speedRegulator);
    MotorCommanderGoal goal(3210, 5432);

    commander.setGoal(goal, 123);

    TEST_ASSERT_EQUAL(goal.targetPositionClicks, commander.goalPos.getLinearPosition());
    TEST_ASSERT_EQUAL(5432 + 123, commander.timeGoalMicros);
    TEST_ASSERT_EQUAL(true, speedRegulator.isRunning);
}

void testConfigure()
{
    RotationalPositionProvider *posProvider = (RotationalPositionProvider *)1234;
    SpeedRegulatorMock speedRegulator;
    int16_t maxSpeedCPS = 1234;

    MotorSpeedCommander commander(posProvider, &speedRegulator);
    commander.config(maxSpeedCPS);

    TEST_ASSERT_EQUAL(posProvider, commander.currentRotPos);
    TEST_ASSERT_EQUAL(&speedRegulator, commander.speedRegulator);
    TEST_ASSERT_EQUAL(maxSpeedCPS, commander.maxSpeedCPS);
}

void testComputeTargetSpeed()
{
    RotationalPositionProvider *posProvider = (RotationalPositionProvider *)1234;
    SpeedRegulatorInterface *speedRegulator = (SpeedRegulatorInterface *)4321;
    MotorSpeedCommander commander(posProvider, speedRegulator);
    int32_t timeLeftMicros = 1e6;
    int32_t clicksLeft = 3000;
    TEST_ASSERT_EQUAL(3000, commander.computeTargetSpeedCPS(timeLeftMicros, clicksLeft));
}

void testComputeTargetSpeedAtTimeZero()
{
    RotationalPositionProvider *posProvider = (RotationalPositionProvider *)1234;
    SpeedRegulatorInterface *speedRegulator = (SpeedRegulatorInterface *)4321;
    MotorSpeedCommander commander(posProvider, speedRegulator);
    int16_t maxSpeedCPS = 1234;
    commander.config(maxSpeedCPS);

    int32_t timeLeftMicros = 0;
    int32_t clicksLeft = 3000;
    TEST_ASSERT_EQUAL(maxSpeedCPS, commander.computeTargetSpeedCPS(timeLeftMicros, clicksLeft));
}

void testComputeTargetSpeedNegativeTimeLeft()
{
    RotationalPositionProvider *posProvider = (RotationalPositionProvider *)1234;
    SpeedRegulatorInterface *speedRegulator = (SpeedRegulatorInterface *)4321;
    int16_t maxSpeedCPS = 1234;

    MotorSpeedCommander commander(posProvider, speedRegulator);
    commander.config(maxSpeedCPS);

    int32_t timeLeftMicros = -1000;
    int32_t clicksLeft = 1000;
    TEST_ASSERT_EQUAL(maxSpeedCPS, commander.computeTargetSpeedCPS(timeLeftMicros, clicksLeft));
}

void testComputeTargetSpeedCapSpeed()
{
    RotationalPositionProvider *posProvider = (RotationalPositionProvider *)1234;
    SpeedRegulatorInterface *speedRegulator = (SpeedRegulatorInterface *)4321;
    int16_t maxSpeedCPS = 123;

    MotorSpeedCommander commander(posProvider, speedRegulator);
    commander.config(maxSpeedCPS);

    int32_t timeLeftMicros = 1000;
    int32_t clicksLeft = 1000;
    TEST_ASSERT_EQUAL(maxSpeedCPS, commander.computeTargetSpeedCPS(timeLeftMicros, clicksLeft));
}

void testRunAlreadyPassedGoal()
{
    RotationalPosition posNow(1234);
    int16_t posGoal = 1234 + 500;
    SpeedRegulatorMock speedRegulator;
    int16_t maxSpeedCPS = 5000;
    int32_t timeNow = 4321;
    int32_t timeGoalMicros = 200;

    MotorCommanderGoal goal(posGoal, timeGoalMicros);

    MotorSpeedCommander commander(&posNow, &speedRegulator);
    commander.config(maxSpeedCPS);
    commander.start();

    commander.setGoal(goal, timeNow);
    commander.goalPos.linPos = 100;

    commander.run(timeNow);

    TEST_ASSERT_EQUAL(0, speedRegulator.setPoint);
    TEST_ASSERT_TRUE(commander.hasArrived());
}

void testRunSimple()
{
    RotationalPosition posNow(1234);
    int16_t posGoal = 1234 + 500;
    SpeedRegulatorMock speedRegulator;
    int16_t maxSpeedCPS = 5000;
    int32_t timeNow = 4321;
    int32_t timeGoalMicros = 250 * 1000L;

    MotorCommanderGoal goal(posGoal, timeGoalMicros);

    MotorSpeedCommander commander(&posNow, &speedRegulator);
    commander.config(maxSpeedCPS);
    commander.setGoal(goal, timeNow);

    commander.stop();
    commander.run(timeNow);
    TEST_ASSERT_EQUAL(1234, speedRegulator.setPoint); // Nothing happened.

    commander.start();
    commander.run(timeNow);
    TEST_ASSERT_EQUAL(2000, speedRegulator.setPoint);
    TEST_ASSERT_FALSE(commander.hasArrived());
}

void testRunOverTime()
{

    RotationalPosition posNow(1234);
    int16_t posGoal = 1234 + 100;
    SpeedRegulatorMock speedRegulator;
    int16_t maxSpeedCPS = 5000;
    int32_t timeNow = 4321;
    int32_t timeGoalMicros = -250;
    MotorCommanderGoal goal(posGoal, timeGoalMicros);

    MotorSpeedCommander commander(&posNow, &speedRegulator);
    commander.config(maxSpeedCPS);
    commander.start();
    commander.setGoal(goal, timeNow);
    commander.run(timeNow);

    TEST_ASSERT_EQUAL(maxSpeedCPS, speedRegulator.setPoint);
    TEST_ASSERT_FALSE(commander.hasArrived());
}

void runAllTestsMotorSpeedCommander()
{
    UNITY_BEGIN_INT();

    RUN_TEST(testSetGoal);
    RUN_TEST(testConfigure);
    RUN_TEST(testComputeTargetSpeed);
    RUN_TEST(testComputeTargetSpeedAtTimeZero);
    RUN_TEST(testComputeTargetSpeedNegativeTimeLeft);
    RUN_TEST(testComputeTargetSpeedCapSpeed);
    RUN_TEST(testRunAlreadyPassedGoal);
    RUN_TEST(testRunSimple);
    RUN_TEST(testRunOverTime);
    UNITY_END_INT();
}