#include <unity.h>

#include <HardwareInterface.h>
#define private public
#include <MotorDriver.h>

void setUp(void) {
    HardwareInterface::resetMicrosecondsSinceBoot();
    HardwareInterface::resetValues();
    
}

void testConfig()
{
    //Do the config
    MotorDriver motorDriver = MotorDriver();
    motorDriver.config( 3, 5, 7 );

    //Check the results
    TEST_ASSERT_EQUAL( HardwareInterface::OUTPUT, HardwareInterface::getPinMode(3) );
    TEST_ASSERT_EQUAL( HardwareInterface::OUTPUT, HardwareInterface::getPinMode(5) );
    TEST_ASSERT_EQUAL( HardwareInterface::OUTPUT, HardwareInterface::getPinMode(7) );    
    
    TEST_ASSERT_EQUAL( 3, motorDriver.driverPinOne );
    TEST_ASSERT_EQUAL( 5, motorDriver.driverPinTwo );
    TEST_ASSERT_EQUAL( 7, motorDriver.driverPinPWM );
    
    TEST_ASSERT_EQUAL( 0, motorDriver.lastMotorPWM );
}

void testSetMotorPWMHardStopFromForward()
{
    //Do the config
    MotorDriver motorDriver = MotorDriver();
    motorDriver.config( 3, 5, 7 );
    motorDriver.lastMotorPWM = 10;
    
    //Stop the motor
    motorDriver.setMotorPWM(0);

    //Check the results
    TEST_ASSERT_EQUAL( HardwareInterface::HIGH, HardwareInterface::pinStatuses[3] );
    TEST_ASSERT_EQUAL( HardwareInterface::LOW, HardwareInterface::pinStatuses[5] );
    TEST_ASSERT_EQUAL( 0, HardwareInterface::pinStatuses[0] );
    TEST_ASSERT_EQUAL( 0, motorDriver.lastMotorPWM );
}

void testSetMotorPWMHardStopFromBackwards()
{
    //Do the config
    MotorDriver motorDriver = MotorDriver();
    motorDriver.config( 3, 5, 7 );
    motorDriver.lastMotorPWM = -10;
    
    //Stop the motor
    motorDriver.setMotorPWM(0);

    //Check the results
    TEST_ASSERT_EQUAL( HardwareInterface::LOW, HardwareInterface::pinStatuses[3] );
    TEST_ASSERT_EQUAL( HardwareInterface::HIGH, HardwareInterface::pinStatuses[5] );
    TEST_ASSERT_EQUAL( 0, HardwareInterface::pinStatuses[0] );
    TEST_ASSERT_EQUAL( 0, motorDriver.lastMotorPWM );
}

void testSetMotorPWMForward()
{
    //Do the config
    MotorDriver motorDriver = MotorDriver();
    motorDriver.config( 3, 5, 7 );
    motorDriver.lastMotorPWM = -10;
    
    //Stop the motor
    motorDriver.setMotorPWM(10);

    //Check the results
    TEST_ASSERT_EQUAL( HardwareInterface::LOW, HardwareInterface::pinStatuses[3] );
    TEST_ASSERT_EQUAL( HardwareInterface::HIGH, HardwareInterface::pinStatuses[5] );
    TEST_ASSERT_EQUAL( 10, HardwareInterface::pinStatuses[7] );
    TEST_ASSERT_EQUAL( 10, motorDriver.lastMotorPWM );
}

void testSetMotorPWMBackward()
{
    //Do the config
    MotorDriver motorDriver = MotorDriver();
    motorDriver.config( 3, 5, 7 );
    motorDriver.lastMotorPWM = 10;
    
    //Stop the motor
    motorDriver.setMotorPWM(-10);

    //Check the results
    TEST_ASSERT_EQUAL( HardwareInterface::HIGH, HardwareInterface::pinStatuses[3] );
    TEST_ASSERT_EQUAL(  HardwareInterface::LOW, HardwareInterface::pinStatuses[5] );
    TEST_ASSERT_EQUAL( 10, HardwareInterface::pinStatuses[7] );
    TEST_ASSERT_EQUAL( -10, motorDriver.lastMotorPWM );
}

void process()
{
    UNITY_BEGIN();  
    RUN_TEST( testConfig );
    RUN_TEST( testSetMotorPWMHardStopFromForward );
    RUN_TEST( testSetMotorPWMHardStopFromBackwards );
    RUN_TEST( testSetMotorPWMForward );
    RUN_TEST( testSetMotorPWMBackward );
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
