#include <unityEx.h>

#define private public
#include <MotorSpeedCommander.h>

void testSetGoal()
{
    MotorSpeedCommander commander;
    RotationalPosition pos ( 1234 );

    commander.setGoal( pos, 4321 );

    TEST_ASSERT( pos == commander.positionGoal );
    TEST_ASSERT_EQUAL( 4321, commander.timeGoal );
}

void testConfigure()
{
    RotationalPositionProvider* pos = (RotationalPositionProvider*)1234;
    SpeedRegulatorInterface* speedRegulator = (SpeedRegulatorInterface*)4321;
    int maxSpeedCPS = 1234;
    
    MotorSpeedCommander commander;
    commander.configure( pos, speedRegulator, maxSpeedCPS );

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
    commander.configure( pos, speedRegulator, maxSpeedCPS );

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
    commander.configure( pos, speedRegulator, maxSpeedCPS );

    long timeLeftMicros = 1000;    
    long clicksLeft = 1000;
    TEST_ASSERT_EQUAL( maxSpeedCPS, commander.computeTargetSpeedCPS( timeLeftMicros, clicksLeft ) );
}

void runAllTestsMotorSpeedCommander()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testSetGoal );
    RUN_TEST( testConfigure );
    RUN_TEST( testComputeTargetSpeed );
    RUN_TEST( testComputeTargetSpeedNegativeTimeLeft );
    RUN_TEST( testComputeTargetSpeedCapSpeed );
    UNITY_END_INT();
}