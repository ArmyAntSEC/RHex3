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

void runAllTestsMotorSpeedCommander()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testSetGoal );
    UNITY_END_INT();
}