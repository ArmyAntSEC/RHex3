#include <unity.h>

#include <HardwareInterface.h>
#define private public
#include <MotorSpeedRegulator.h>
#include "MockSpeedometer.h"
#include "MockMotorDriver.h"

void setUp(void) {
    HardwareInterface::resetMicrosecondsSinceBoot();
    HardwareInterface::resetPins();
}

void testInit()
{
    MotorSpeedRegulator regulator;
    MockSpeedometer speedometer;
    MockMotorDriver driver;
    SpeedToPowerConverterProduction* converter = (SpeedToPowerConverterProduction*)3;
    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    
    regulator.config(&speedometer, &driver, converter, P, D, I, filter );
    
    regulator.init();

    TEST_ASSERT_EQUAL( true, regulator.running );
    TEST_ASSERT_EQUAL( 3, regulator.input );
    TEST_ASSERT_EQUAL( 3, regulator.lastInput );    
    TEST_ASSERT_EQUAL( driver.getMotorPWM(), regulator.integratorCumulativeValue );

}

void testConfig()
{
    MotorSpeedRegulator regulator;
    SpeedometerInterface* speedometer = (SpeedometerInterface*)1;
    MockMotorDriver driver;
    SpeedToPowerConverterProduction* converter = (SpeedToPowerConverterProduction*)3;
    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    
    regulator.config(speedometer, &driver, converter, P, D, I, filter );

    TEST_ASSERT_EQUAL( speedometer, regulator.speedometer );
    TEST_ASSERT_EQUAL( &driver, regulator.driver );
    TEST_ASSERT_EQUAL( converter, regulator.converter );
    TEST_ASSERT_EQUAL_FLOAT( P, regulator.proportionalTerm );
    TEST_ASSERT_EQUAL_FLOAT( D, regulator.derivativeTerm );
    TEST_ASSERT_EQUAL_FLOAT( I, regulator.integratorTerm );
    TEST_ASSERT_EQUAL_FLOAT( filter, regulator.filter );

}

void testStart()
{
    TEST_IGNORE();
}

void testStop()
{
    TEST_IGNORE();
}

void testClampOutput()
{
    TEST_IGNORE();
}
    
void testClampOutputForSpeed()
{
    TEST_IGNORE();
}
void testDoCorePIDAlgorithmStepClampedForSpeed()
{
    TEST_IGNORE();
}
    
void testHandleHardBreak ()
{
    TEST_IGNORE();
}
    
void testRun()
{
    TEST_IGNORE();
}

void process()
{
    UNITY_BEGIN();  
    RUN_TEST( testInit );
    RUN_TEST( testConfig );
    RUN_TEST( testStart );
    RUN_TEST( testStop );
    RUN_TEST( testClampOutput );
    RUN_TEST( testClampOutputForSpeed );
    RUN_TEST( testDoCorePIDAlgorithmStepClampedForSpeed );
    RUN_TEST( testHandleHardBreak );
    RUN_TEST ( testRun );

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
