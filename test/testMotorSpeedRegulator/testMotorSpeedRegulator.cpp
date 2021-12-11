#include <unity.h>

#include <HardwareInterface.h>
#define private public
#include <MotorSpeedRegulator.h>
#include "MockSpeedometer.h"
#include "MockMotorDriver.h"
#include "MockEEPROMBackedArray.h"

void setUp(void) {
    HardwareInterface::resetMicrosecondsSinceBoot();
    HardwareInterface::resetValues();
}

void testInit()
{
    MotorSpeedRegulator regulator;
    MockSpeedometer speedometer;
    MockMotorDriver driver;
    SpeedToPowerConverter* converter = 0;
    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    
    regulator.config(&speedometer, &driver, converter, P, D, I, filter );
    
    regulator.init();

    TEST_ASSERT_EQUAL( true, regulator.running );
    TEST_ASSERT_EQUAL( speedometer.speedCPSFiltered, regulator.input );
    TEST_ASSERT_EQUAL( speedometer.speedCPSFiltered, regulator.lastInput );    
    TEST_ASSERT_EQUAL( driver.getMotorPWM(), regulator.integratorCumulativeValue );

}

void testConfig()
{
    MotorSpeedRegulator regulator;
    SpeedometerInterface* speedometer = 0;
    MockMotorDriver driver;
    SpeedToPowerConverter* converter = 0;
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
    MockMotorDriver driver;
    SpeedToPowerConverter* converter = 0;
    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    regulator.integratorCumulativeValue = 5;
    
    regulator.config(&speedometer, &driver, converter, P, D, I, filter );

    regulator.start();

    TEST_ASSERT_TRUE( regulator.isOn );
    TEST_ASSERT_TRUE( regulator.running );
    TEST_ASSERT_EQUAL( speedometer.speedCPSFiltered, regulator.lastInput );
    TEST_ASSERT_EQUAL( driver.motorPWM, regulator.integratorCumulativeValue );
}

void testStop()
{
    MotorSpeedRegulator regulator;
    SpeedometerInterface* speedometer = 0;
    MockMotorDriver driver;
    SpeedToPowerConverter* converter = 0;
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
    MotorSpeedRegulator regulator;
    SpeedometerInterface* speedometer = 0;
    MockMotorDriver* driver = 0;
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );

    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    
    regulator.config(speedometer, driver, &converter, P, D, I, filter );

    TEST_ASSERT_EQUAL( regulator.maxOutput/2, regulator.clampOutputForSpeed(regulator.maxOutput/2, 13) );
    TEST_ASSERT_EQUAL( regulator.maxOutput, regulator.clampOutputForSpeed(regulator.maxOutput*2, 13) );
    TEST_ASSERT_EQUAL( 13*0.6, regulator.clampOutputForSpeed(1, 11) );
}

void testDoCorePIDAlgorithmStepClampedForSpeedNoClamping()
{
    int setPoint = 2100;
    int oldSpeed = 2000;
    int newSpeed = 2050;

    MotorSpeedRegulator regulator;
    MockSpeedometer speedometer;
    speedometer.speedCPSFiltered = oldSpeed;
    

    MockMotorDriver driver;
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );
    float P = 0.5;
    float D = 0.1;
    float I = 0.1;
    float filter = 7;
    
    regulator.config(&speedometer, &driver, &converter, P, D, I, filter );
    regulator.init();
    regulator.start();
    TEST_ASSERT_EQUAL( oldSpeed, regulator.lastInput );

    speedometer.speedCPSFiltered = newSpeed;

    regulator.setSetPoint( setPoint );

    regulator.doCorePIDAlgorithmStepClampedForSpeed();

    TEST_ASSERT_EQUAL( newSpeed, regulator.input );
    int expectedIntegratorCumulativeValue = driver.motorPWM + I*(setPoint-newSpeed);
    TEST_ASSERT_EQUAL( expectedIntegratorCumulativeValue, regulator.integratorCumulativeValue );
    TEST_ASSERT_EQUAL( newSpeed, regulator.lastInput );    
    int expectedDiffOfInput = newSpeed - oldSpeed;
    TEST_ASSERT_EQUAL( expectedDiffOfInput, regulator.diffOfInput );
    int expectedErrorTerm = setPoint - newSpeed;
    TEST_ASSERT_EQUAL( expectedErrorTerm, regulator.errorTerm );
    int expectedOutput = P*expectedErrorTerm  + D*expectedDiffOfInput + expectedIntegratorCumulativeValue; 
    TEST_ASSERT_EQUAL( expectedOutput, regulator.output );
}

void testDoCorePIDAlgorithmStepClampedForSpeedIntegratorUpwardsClamping()
{
    int setPoint = 1800;
    int oldSpeed = 2000;
    int newSpeed = 2050;

    MotorSpeedRegulator regulator;
    MockSpeedometer speedometer;
    speedometer.speedCPSFiltered = oldSpeed;
    

    MockMotorDriver driver;
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );
    float P = 0.5;
    float D = 0.1;
    float I = 0.1;
    float filter = 7;
    
    regulator.config(&speedometer, &driver, &converter, P, D, I, filter );
    regulator.init();
    regulator.start();
    TEST_ASSERT_EQUAL( oldSpeed, regulator.lastInput );

    speedometer.speedCPSFiltered = newSpeed;

    regulator.setSetPoint( setPoint );
    regulator.integratorCumulativeValue = 2500;

    regulator.doCorePIDAlgorithmStepClampedForSpeed();

    TEST_ASSERT_EQUAL( newSpeed, regulator.input );
    int expectedIntegratorCumulativeValue = 255;
    TEST_ASSERT_EQUAL( expectedIntegratorCumulativeValue, regulator.integratorCumulativeValue );
    TEST_ASSERT_EQUAL( newSpeed, regulator.lastInput );    
    int expectedDiffOfInput = newSpeed - oldSpeed;
    TEST_ASSERT_EQUAL( expectedDiffOfInput, regulator.diffOfInput );
    int expectedErrorTerm = setPoint - newSpeed;
    TEST_ASSERT_EQUAL( expectedErrorTerm, regulator.errorTerm );
    int expectedOutput = P*expectedErrorTerm  + D*expectedDiffOfInput + expectedIntegratorCumulativeValue; 
    TEST_ASSERT_EQUAL( expectedOutput, regulator.output );
}

void testDoCorePIDAlgorithmStepClampedForSpeedIntegratorDownwardsClamping()
{
    TEST_IGNORE();
    
    int setPoint = 1800;
    int oldSpeed = 2000;
    int newSpeed = 2050;

    MotorSpeedRegulator regulator;
    MockSpeedometer speedometer;
    speedometer.speedCPSFiltered = oldSpeed;
    

    MockMotorDriver driver;
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );
    float P = 0.5;
    float D = 0.1;
    float I = 0.1;
    float filter = 7;
    
    regulator.config(&speedometer, &driver, &converter, P, D, I, filter );
    regulator.init();
    regulator.start();
    TEST_ASSERT_EQUAL( oldSpeed, regulator.lastInput );

    speedometer.speedCPSFiltered = newSpeed;

    regulator.setSetPoint( setPoint );
    regulator.integratorCumulativeValue = 2500;

    regulator.doCorePIDAlgorithmStepClampedForSpeed();

    TEST_ASSERT_EQUAL( newSpeed, regulator.input );
    int expectedIntegratorCumulativeValue = 255;
    TEST_ASSERT_EQUAL( expectedIntegratorCumulativeValue, regulator.integratorCumulativeValue );
    TEST_ASSERT_EQUAL( newSpeed, regulator.lastInput );    
    int expectedDiffOfInput = newSpeed - oldSpeed;
    TEST_ASSERT_EQUAL( expectedDiffOfInput, regulator.diffOfInput );
    int expectedErrorTerm = setPoint - newSpeed;
    TEST_ASSERT_EQUAL( expectedErrorTerm, regulator.errorTerm );
    int expectedOutput = P*expectedErrorTerm  + D*expectedDiffOfInput + expectedIntegratorCumulativeValue; 
    TEST_ASSERT_EQUAL( expectedOutput, regulator.output );
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
    RUN_TEST( testDoCorePIDAlgorithmStepClampedForSpeedNoClamping );
    RUN_TEST( testDoCorePIDAlgorithmStepClampedForSpeedIntegratorUpwardsClamping );
    RUN_TEST( testDoCorePIDAlgorithmStepClampedForSpeedIntegratorDownwardsClamping );
    RUN_TEST( testHandleHardBreak );
    RUN_TEST ( testRun );
}

