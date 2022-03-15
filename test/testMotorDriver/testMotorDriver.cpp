#include <unityEx.h>

#include <HardwarePins.h>
#include <HardwareClock.h>

#define private public
#include <MotorDriver.h>

HardwareClockMock HWClock;
HardwarePinsMock HWPins;

void setUp(void) {
    HWClock.resetMicrosecondsSinceBoot();   
    HWPins.resetValues(); 
}

void testConfig()
{
    //Do the config
    MotorDriver motorDriver = MotorDriver();
    motorDriver.config( 3, 5, 7, &HWPins );

    //Check the results
    TEST_ASSERT_EQUAL( HWPins.OUTPUT, HWPins.pinModes[3] );
    TEST_ASSERT_EQUAL( HWPins.OUTPUT, HWPins.pinModes[5] );
    TEST_ASSERT_EQUAL( HWPins.OUTPUT, HWPins.pinModes[7] );    
    
    TEST_ASSERT_EQUAL( 3, motorDriver.driverPinOne );
    TEST_ASSERT_EQUAL( 5, motorDriver.driverPinTwo );
    TEST_ASSERT_EQUAL( 7, motorDriver.driverPinPWM );
    
    TEST_ASSERT_EQUAL( 0, motorDriver.lastMotorPWM );
}

void testSetMotorPWMHardStopFromForward()
{
    //Do the config
    MotorDriver motorDriver = MotorDriver();
    motorDriver.config( 3, 5, 7, &HWPins );
    motorDriver.lastMotorPWM = 10;
    
    //Stop the motor
    motorDriver.setMotorPWM(0);

    //Check the results
    TEST_ASSERT_EQUAL( HWPins.HIGH, HWPins.pinStatuses[3] );
    TEST_ASSERT_EQUAL( HWPins.LOW, HWPins.pinStatuses[5] );
    TEST_ASSERT_EQUAL( 0, HWPins.pinStatuses[7] );
    TEST_ASSERT_EQUAL( 0, motorDriver.lastMotorPWM );
}

void testSetMotorPWMHardStopFromBackwards()
{
    //Do the config
    MotorDriver motorDriver = MotorDriver();
    motorDriver.config( 3, 5, 7, &HWPins );
    motorDriver.lastMotorPWM = -10;
    
    //Stop the motor
    motorDriver.setMotorPWM(0);

    //Check the results
    TEST_ASSERT_EQUAL( HWPins.LOW, HWPins.pinStatuses[3] );
    TEST_ASSERT_EQUAL( HWPins.HIGH, HWPins.pinStatuses[5] );    
    TEST_ASSERT_EQUAL( 0, HWPins.pinStatuses[7] );
    TEST_ASSERT_EQUAL( 0, motorDriver.lastMotorPWM );
}

void testSetMotorPWMForward()
{
    //Do the config
    MotorDriver motorDriver = MotorDriver();
    motorDriver.config( 3, 5, 7, &HWPins );
    motorDriver.lastMotorPWM = -10;
    
    //Stop the motor
    motorDriver.setMotorPWM(10);

    //Check the results
    TEST_ASSERT_EQUAL( HWPins.LOW, HWPins.pinStatuses[3] );
    TEST_ASSERT_EQUAL( HWPins.HIGH, HWPins.pinStatuses[5] );
    TEST_ASSERT_EQUAL( 10, HWPins.pinStatuses[7] );
    TEST_ASSERT_EQUAL( 10, motorDriver.lastMotorPWM );
}

void testSetMotorPWMBackward()
{
    //Do the config
    MotorDriver motorDriver = MotorDriver();
    motorDriver.config( 3, 5, 7, &HWPins );
    motorDriver.lastMotorPWM = 10;
    
    //Stop the motor
    motorDriver.setMotorPWM(-10);

    //Check the results
    TEST_ASSERT_EQUAL( HWPins.HIGH, HWPins.pinStatuses[3] );
    TEST_ASSERT_EQUAL(  HWPins.LOW, HWPins.pinStatuses[5] );
    TEST_ASSERT_EQUAL( 10, HWPins.pinStatuses[7] );
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
