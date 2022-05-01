#include <unityEx.h>
#define private public
#include <LegCommandReceiver.h>


void testCreateLegCommandReceiver()
{
    TEST_FAIL();
}


void runAllTestsLegCommandREceiver()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testCreateLegCommandReceiver );    
    UNITY_END_INT();
}