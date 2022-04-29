#include <unityEx.h>

#define private public
#include <MotorCommanderGoalSerializer.h>

void testSerializeGoal()
{
    MotorCommanderGoalSerializer sut;
    MotorCommanderGoal goal( 1234, 4567 );
    char buffer[64];

    sut.serialize( goal, buffer, 64 );

    TEST_ASSERT_EQUAL_STRING( "{\"targetPositionClicks\":1234,\"targetTimeMicros\":4567}", buffer );
}

void testDeserializeGoal()
{
    MotorCommanderGoalSerializer sut;    
    char buffer[64] = "{\"targetPositionClicks\":1234,\"targetTimeMicros\":4567}";

    MotorCommanderGoal goal = sut.deserialize( buffer );

    TEST_ASSERT_EQUAL ( 1234, goal.targetPositionClicks );
    TEST_ASSERT_EQUAL ( 4567, goal.targetTimeMicros );
}


void runAllTestSpeedCommanderGoalSerializer()
{    
    UNITY_BEGIN_INT();    
    RUN_TEST( testSerializeGoal );
    RUN_TEST( testDeserializeGoal );
    UNITY_END_INT();

}