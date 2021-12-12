#include <unity.h>

#define private public
#include <MotorSpeedRegulator.h>
#include <MockHomingEncoder.h>
#include <MockMotorDriver.h>
#include <MockEEPROMBackedArray.h>
#include "TestingMotorSpeedRegulator.h"

void testInit()
{
    MotorSpeedRegulator regulator;
    MockHomingEncoder homingEncoder;
    MockMotorDriver driver;
    SpeedToPowerConverter* converter = 0;
    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    
    regulator.config(&homingEncoder, &driver, converter, P, D, I, filter );
    
    regulator.init();

    TEST_ASSERT_EQUAL( true, regulator.running );
    TEST_ASSERT_EQUAL( homingEncoder.speedCPSFiltered, regulator.input );
    TEST_ASSERT_EQUAL( homingEncoder.speedCPSFiltered, regulator.lastInput );    
    TEST_ASSERT_EQUAL( driver.getMotorPWM(), regulator.integratorCumulativeValue );

}

void testConfig()
{
    MotorSpeedRegulator regulator;
    MockHomingEncoder* homingEncoder = 0;
    MockMotorDriver driver;
    SpeedToPowerConverter* converter = 0;
    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    
    regulator.config(homingEncoder, &driver, converter, P, D, I, filter );

    TEST_ASSERT_EQUAL( homingEncoder, regulator.homingEncoder );
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
    MockHomingEncoder homingEncoder;
    MockMotorDriver driver;
    SpeedToPowerConverter* converter = 0;
    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    regulator.integratorCumulativeValue = 5;
    
    regulator.config(&homingEncoder, &driver, converter, P, D, I, filter );

    regulator.start();

    TEST_ASSERT_TRUE( regulator.isOn );
    TEST_ASSERT_TRUE( regulator.running );
    TEST_ASSERT_EQUAL( homingEncoder.speedCPSFiltered, regulator.lastInput );
    TEST_ASSERT_EQUAL( driver.motorPWM, regulator.integratorCumulativeValue );
}

void testStop()
{
    MotorSpeedRegulator regulator;
    HomingEncoderInterface* homingEncoder = 0;
    MockMotorDriver driver;
    SpeedToPowerConverter* converter = 0;
    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    driver.setMotorPWM(5);
    
    regulator.config(homingEncoder, &driver, converter, P, D, I, filter );

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
    HomingEncoderInterface* homingEncoder = 0;
    MockMotorDriver* driver = 0;
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );

    float P = 4;
    float D = 5;
    float I = 6;
    float filter = 7;
    
    regulator.config(homingEncoder, driver, &converter, P, D, I, filter );

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
    MockHomingEncoder homingEncoder;
    homingEncoder.speedCPSFiltered = oldSpeed;
    

    MockMotorDriver driver;
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );
    float P = 0.5;
    float D = 0.1;
    float I = 0.1;
    float filter = 7;
    
    regulator.config(&homingEncoder, &driver, &converter, P, D, I, filter );
    regulator.init();
    regulator.start();
    TEST_ASSERT_EQUAL( oldSpeed, regulator.lastInput );

    homingEncoder.speedCPSFiltered = newSpeed;

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
    MockHomingEncoder homingEncoder;
    homingEncoder.speedCPSFiltered = oldSpeed;
    

    MockMotorDriver driver;
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );
    float P = 0.5;
    float D = 0.1;
    float I = 0.1;
    float filter = 7;
    
    regulator.config(&homingEncoder, &driver, &converter, P, D, I, filter );
    regulator.init();
    regulator.start();
    TEST_ASSERT_EQUAL( oldSpeed, regulator.lastInput );

    homingEncoder.speedCPSFiltered = newSpeed;

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
    int setPoint = 2100;
    int oldSpeed = 2000;
    int newSpeed = 2050;

    MotorSpeedRegulator regulator;
    MockHomingEncoder homingEncoder;
    homingEncoder.speedCPSFiltered = oldSpeed;
    

    MockMotorDriver driver;
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );
    float P = 0.5;
    float D = 0.1;
    float I = 0.1;
    float filter = 7;
    
    regulator.config(&homingEncoder, &driver, &converter, P, D, I, filter );
    regulator.init();
    regulator.start();
    TEST_ASSERT_EQUAL( oldSpeed, regulator.lastInput );

    homingEncoder.speedCPSFiltered = newSpeed;

    regulator.setSetPoint( setPoint );
    regulator.integratorCumulativeValue = 0;

    regulator.doCorePIDAlgorithmStepClampedForSpeed();

    TEST_ASSERT_EQUAL( newSpeed, regulator.input );
    int expectedIntegratorCumulativeValue = converter.GetPowerForFreeSpeed(setPoint)*0.6;
    TEST_ASSERT_EQUAL( expectedIntegratorCumulativeValue, regulator.integratorCumulativeValue );
    TEST_ASSERT_EQUAL( newSpeed, regulator.lastInput );    
    int expectedDiffOfInput = newSpeed - oldSpeed;
    TEST_ASSERT_EQUAL( expectedDiffOfInput, regulator.diffOfInput );
    int expectedErrorTerm = setPoint - newSpeed;
    TEST_ASSERT_EQUAL( expectedErrorTerm, regulator.errorTerm );
    int expectedOutput = P*expectedErrorTerm  + D*expectedDiffOfInput + expectedIntegratorCumulativeValue; 
    TEST_ASSERT_EQUAL( expectedOutput, regulator.output );
}

void testDoCorePIDAlgorithmStepClampedForSpeedOutputUpwardsClamping()
{
    int setPoint = 3000;
    int oldSpeed = 2000;
    int newSpeed = 2050;

    MotorSpeedRegulator regulator;
    MockHomingEncoder homingEncoder;
    homingEncoder.speedCPSFiltered = oldSpeed;
    

    MockMotorDriver driver;
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );
    float P = 0.5;
    float D = 0.1;
    float I = 0.1;
    float filter = 7;
    
    regulator.config(&homingEncoder, &driver, &converter, P, D, I, filter );
    regulator.init();
    regulator.start();
    TEST_ASSERT_EQUAL( oldSpeed, regulator.lastInput );

    homingEncoder.speedCPSFiltered = newSpeed;

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
    int expectedOutput = 255; 
    TEST_ASSERT_EQUAL( expectedOutput, regulator.output );
}

void testDoCorePIDAlgorithmStepClampedForSpeedOutputDownwardsClamping()
{
    int setPoint = 000;
    int oldSpeed = 2000;
    int newSpeed = 2050;

    MotorSpeedRegulator regulator;
    MockHomingEncoder homingEncoder;
    homingEncoder.speedCPSFiltered = oldSpeed;
    

    MockMotorDriver driver;
    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );
    float P = 0.5;
    float D = 0.1;
    float I = 0.1;
    float filter = 7;
    
    regulator.config(&homingEncoder, &driver, &converter, P, D, I, filter );
    regulator.init();
    regulator.start();
    TEST_ASSERT_EQUAL( oldSpeed, regulator.lastInput );

    homingEncoder.speedCPSFiltered = newSpeed;

    regulator.setSetPoint( setPoint );

    regulator.doCorePIDAlgorithmStepClampedForSpeed();

    TEST_ASSERT_EQUAL( newSpeed, regulator.input );
    TEST_ASSERT_EQUAL( 1, regulator.integratorCumulativeValue );
    TEST_ASSERT_EQUAL( newSpeed, regulator.lastInput );    
    int expectedDiffOfInput = newSpeed - oldSpeed;
    TEST_ASSERT_EQUAL( expectedDiffOfInput, regulator.diffOfInput );
    int expectedErrorTerm = setPoint - newSpeed;
    TEST_ASSERT_EQUAL( expectedErrorTerm, regulator.errorTerm );
    int expectedOutput = 1; 
    TEST_ASSERT_EQUAL( expectedOutput, regulator.output );
}

void testHandleHardBreakNoHardBreakMode ()
{
    MotorSpeedRegulator regulator;
    regulator.input = 2500;
    regulator.setPoint = 2000;
    regulator.integratorCumulativeValue = 2700;
    regulator.output = 165;
    regulator.lastInput = 2400;
    regulator.hardBreakMode = false;

    regulator.handleHardBreak();

    TEST_ASSERT_EQUAL( 2700, regulator.integratorCumulativeValue );
    TEST_ASSERT_EQUAL( 165, regulator.output );
    TEST_ASSERT_EQUAL( 2400, regulator.lastInput );
}

void testHandleHardBreakTooFast ()
{
    MotorSpeedRegulator regulator;
    regulator.input = 2500;
    regulator.setPoint = 2000;
    regulator.integratorCumulativeValue = 2700;
    regulator.output = 165;
    regulator.lastInput = 2400;
    regulator.hardBreakMode = true;

    regulator.handleHardBreak();

    TEST_ASSERT_EQUAL( 0, regulator.integratorCumulativeValue );
    TEST_ASSERT_EQUAL( 0, regulator.output );
    TEST_ASSERT_EQUAL( regulator.input, regulator.lastInput );

}

void testHandleHardBreakTooSlow ()
{
    MotorSpeedRegulator regulator;

    MockEEPROMBackedArray<2,8> array;    
    SpeedToPowerConverter converter( &array );
    regulator.converter = &converter;

    regulator.input = 2000;
    regulator.setPoint = 2500;
    regulator.integratorCumulativeValue = 2700;
    regulator.output = 165;
    regulator.lastInput = 2400;
    regulator.hardBreakMode = true;

    regulator.handleHardBreak();

    TEST_ASSERT_EQUAL( false, regulator.hardBreakMode );
    TEST_ASSERT_EQUAL( regulator.input, regulator.lastInput );
    TEST_ASSERT_EQUAL( converter.GetPowerForFreeSpeed(regulator.setPoint), regulator.integratorCumulativeValue );
}

void testRunWhenOn()
{
    TestingMotorSpeedRegulator regulator;
    MockMotorDriver driver;
    regulator.driver = &driver;

    regulator.output = 123;
    regulator.isOn = true;

    regulator.run(0);

    TEST_ASSERT_EQUAL( 123, driver.motorPWM );
    TEST_ASSERT_EQUAL( 1, regulator.countDoCorePIDAlgorithmStepClampedForSpeed );
    TEST_ASSERT_EQUAL( 1, regulator.countHandleHardBreak );
}

void testRunWhenOff()
{
    TestingMotorSpeedRegulator regulator;
    MockMotorDriver driver;
    driver.motorPWM = 0;
    regulator.driver = &driver;

    regulator.output = 123;
    regulator.isOn = false;

    regulator.run(0);

    TEST_ASSERT_EQUAL( 0, driver.motorPWM );
    TEST_ASSERT_EQUAL( 0, regulator.countDoCorePIDAlgorithmStepClampedForSpeed );
    TEST_ASSERT_EQUAL( 0, regulator.countHandleHardBreak );
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
    RUN_TEST( testDoCorePIDAlgorithmStepClampedForSpeedOutputUpwardsClamping );
    RUN_TEST( testDoCorePIDAlgorithmStepClampedForSpeedOutputDownwardsClamping );
    RUN_TEST( testHandleHardBreakNoHardBreakMode );
    RUN_TEST( testHandleHardBreakTooFast );
    RUN_TEST( testHandleHardBreakTooSlow );
    RUN_TEST ( testRunWhenOn );
    RUN_TEST ( testRunWhenOff );
}

