#include <unityEx.h>
#include <SerialStream.h>

void testShallControllOneLegForOneRound()
{
    TEST_IGNORE();
}

void runAllTestGaitCommander()
{    
    UNITY_BEGIN_INT();
    RUN_TEST( testShallControllOneLegForOneRound );
    UNITY_END_INT();

}