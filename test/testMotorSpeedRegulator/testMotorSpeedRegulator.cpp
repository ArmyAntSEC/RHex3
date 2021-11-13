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
    SpeedToPowerConverterProduction* converter = 0;
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
    SpeedometerInterface* speedometer = 0;
    MockMotorDriver driver;
    SpeedToPowerConverterProduction* converter = 0;
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
    MotorSpeedRegulator regulator;
    MockSpeedometer speedometer;
    MotorDriverInterface* driver = 0;
    SpeedToPowerConverterProduction* converter = 0;
    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    regulator.integratorCumulativeValue = 5;
    
    regulator.config(&speedometer, driver, converter, P, D, I, filter );

    regulator.start();

    TEST_ASSERT_TRUE( regulator.isOn );
    TEST_ASSERT_TRUE( regulator.running );
    TEST_ASSERT_EQUAL( speedometer.getSpeedCPS(), regulator.lastInput );
    TEST_ASSERT_EQUAL( 0, regulator.integratorCumulativeValue );
}

void testStop()
{
    MotorSpeedRegulator regulator;
    SpeedometerInterface* speedometer = 0;
    MockMotorDriver driver;
    SpeedToPowerConverterProduction* converter = 0;
    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    driver.setMotorPWM(5);
    
    regulator.config(speedometer, &driver, converter, P, D, I, filter );

    regulator.stop();

    TEST_ASSERT_FALSE( regulator.isOn );
    TEST_ASSERT_FALSE( regulator.running );
    TEST_ASSERT_EQUAL( 0, driver.getMotorPWM() );
    TEST_ASSERT_EQUAL( 0, regulator.integratorCumulativeValue );
}

void testClampOutputHigh()
{
    MotorSpeedRegulator regulator;   
    TEST_ASSERT_EQUAL( regulator.maxOutput, regulator.clampOutput(regulator.maxOutput*2) );
}

void testClampOutputLow()
{
    MotorSpeedRegulator regulator;   
    TEST_ASSERT_EQUAL( 1, regulator.clampOutput(-regulator.maxOutput*2) );
}

void testClampOutputZero()
{
    MotorSpeedRegulator regulator;   
    TEST_ASSERT_EQUAL( 0, regulator.clampOutput(0) );
}

void testClampOutputOK()
{
    MotorSpeedRegulator regulator;   
    TEST_ASSERT_EQUAL( regulator.maxOutput/2, regulator.clampOutput(regulator.maxOutput/2) );
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

void processMotorSpeedRegulator()
{    
    RUN_TEST( testInit );
    RUN_TEST( testConfig );
    RUN_TEST( testStart );
    RUN_TEST( testStop );
    RUN_TEST( testClampOutputHigh );
    RUN_TEST( testClampOutputLow );
    RUN_TEST( testClampOutputOK );
    RUN_TEST( testClampOutputZero );
    RUN_TEST( testClampOutputForSpeed );
    RUN_TEST( testDoCorePIDAlgorithmStepClampedForSpeed );
    RUN_TEST( testHandleHardBreak );
    RUN_TEST ( testRun );
}

