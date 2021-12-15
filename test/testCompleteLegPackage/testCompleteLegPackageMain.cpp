#include <unity.h>

#include <CompleteLegPackage.h>

void testGetPositionDefault()
{
    CompleteLegPackage leg;
    RotationPositionWithLaps pos = leg.getPosition();  
    TEST_ASSERT_EQUAL( 0, pos.getClickPosition() );
    TEST_ASSERT_EQUAL( 0, pos.getLaps() );
    TEST_ASSERT_EQUAL( 0, pos.getRemainder() );
}

void process()
{
    UNITY_BEGIN();  
    RUN_TEST( testGetPositionDefault );    
    UNITY_END();  
}

#ifdef ARDUINO

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    HardwareInterface::delayForMilliseconds(2000);

    process();
}

void loop() {
}
#else
int main(void)
{
    process();
}
#endif
