#include <unityEx.h>

#define private public
#include <MotorSpeedCommander.h>

void testSetGoal()
{
    TEST_FAIL();
}

void runAllTestsMotorSpeedCommander()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testSetGoal );
    UNITY_END_INT();
}