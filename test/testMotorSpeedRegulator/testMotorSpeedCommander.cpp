#include <unityEx.h>

#define private public
#include <MotorSpeedCommander.h>

struct SpeedRegulatorMock: public SpeedRegulatorInterface
{
    int setPoint = 1234;

    virtual void setSetPoint( int _setPoint ) 
    {
        setPoint = _setPoint;
    }
};


void testSetGoal()
{
    MotorSpeedCommander commander;
    RotationalPosition pos ( 1234 );

    commander.setGoal( pos, 4321 );

    TEST_ASSERT( pos == commander.positionGoal );
    TEST_ASSERT_EQUAL( 4321, commander.timeGoalMicros );
}

void testConfigure()
{
    RotationalPositionProvider* pos = (RotationalPositionProvider*)1234;
    SpeedRegulatorInterface* speedRegulator = (SpeedRegulatorInterface*)4321;
    int maxSpeedCPS = 1234;
    
    MotorSpeedCommander commander;
    commander.config( pos, speedRegulator, maxSpeedCPS );

    TEST_ASSERT_EQUAL( pos, commander.currentRotPos );    
    TEST_ASSERT_EQUAL( speedRegulator, commander.speedRegulator );    
    TEST_ASSERT_EQUAL( maxSpeedCPS, commander.maxSpeedCPS );    
}

void testComputeTargetSpeed()
{
    MotorSpeedCommander commander;   
    long timeLeftMicros = 1000;    
    long clicksLeft = 1000;
    TEST_ASSERT_EQUAL( 1000, commander.computeTargetSpeedCPS( timeLeftMicros, clicksLeft ) );
}

void testComputeTargetSpeedNegativeTimeLeft()
{
    RotationalPositionProvider* pos;
    SpeedRegulatorInterface* speedRegulator;
    int maxSpeedCPS = 1234;
    
    MotorSpeedCommander commander;
    commander.config( pos, speedRegulator, maxSpeedCPS );

    long timeLeftMicros = -1000;    
    long clicksLeft = 1000;
    TEST_ASSERT_EQUAL( maxSpeedCPS, commander.computeTargetSpeedCPS( timeLeftMicros, clicksLeft ) );
}

void testComputeTargetSpeedCapSpeed()
{
    RotationalPositionProvider* pos;
    SpeedRegulatorInterface* speedRegulator;
    int maxSpeedCPS = 123;
    
    MotorSpeedCommander commander;
    commander.config( pos, speedRegulator, maxSpeedCPS );

    long timeLeftMicros = 1000;    
    long clicksLeft = 1000;
    TEST_ASSERT_EQUAL( maxSpeedCPS, commander.computeTargetSpeedCPS( timeLeftMicros, clicksLeft ) );
}

void testRunAlreadyPassedGoal()
{
    RotationalPosition posNow(1234);
    RotationalPosition posGoal ( 1234 - 500 );
    SpeedRegulatorMock speedRegulator;
    int maxSpeedCPS = 5000;    
    long timeNow = 4321;
    long timeGoalMicros = 4321 + 200;

    MotorSpeedCommander commander;
    commander.config( &posNow, &speedRegulator, maxSpeedCPS );    

    commander.setGoal( posGoal, timeGoalMicros );
    
    commander.run(timeNow);

    TEST_ASSERT_EQUAL( 0, speedRegulator.setPoint );
    TEST_ASSERT_TRUE( commander.hasArrived() );
}

void testRunSimple()
{
    RotationalPosition posNow(1234);
    RotationalPosition posGoal ( 1234 + 500 );
    SpeedRegulatorMock speedRegulator;
    int maxSpeedCPS = 5000;    
    long timeNow = 4321;
    long timeGoalMicros = 4321 + 250;

    MotorSpeedCommander commander;
    commander.config( &posNow, &speedRegulator, maxSpeedCPS );    

    commander.setGoal( posGoal, timeGoalMicros );
    
    commander.run(timeNow);

    TEST_ASSERT_EQUAL( 2000, speedRegulator.setPoint );
    TEST_ASSERT_FALSE( commander.hasArrived() );
}

void testRunOverTime()
{
    RotationalPosition posNow(1234);
    RotationalPosition posGoal ( 1234 + 500 );
    SpeedRegulatorMock speedRegulator;
    int maxSpeedCPS = 5000;    
    long timeNow = 4321;
    long timeGoalMicros = 4321 - 250;

    MotorSpeedCommander commander;
    commander.config( &posNow, &speedRegulator, maxSpeedCPS );    

    commander.setGoal( posGoal, timeGoalMicros );
    
    commander.run(timeNow);

    TEST_ASSERT_EQUAL( maxSpeedCPS, speedRegulator.setPoint );
    TEST_ASSERT_FALSE( commander.hasArrived() );    
}

void runAllTestsMotorSpeedCommander()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testSetGoal );
    RUN_TEST( testConfigure );
    RUN_TEST( testComputeTargetSpeed );
    RUN_TEST( testComputeTargetSpeedNegativeTimeLeft );
    RUN_TEST( testComputeTargetSpeedCapSpeed );
    RUN_TEST( testRunAlreadyPassedGoal );
    RUN_TEST( testRunSimple );
    RUN_TEST( testRunOverTime );
    UNITY_END_INT();
}