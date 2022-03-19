#include <unityEx.h>
//#include <HardwareInterface.h>

#define private public
#include <SpeedRegulator.h>

struct MockSpeedometer: public CanProvideSpeed
{    
    int16_t speed;
    MockSpeedometer()
    {
        init();
    }
    void init()
    {
        speed = 3;
    }

    virtual  int16_t getSpeedCPS() { return speed; }
};

struct MockDriver: public MotorDriverInterface
{
    int16_t driveSignal; 

    MockDriver()
    {
        init();
    }

    void init()
    {
        driveSignal = 5;
    }

    virtual void setMotorPWM( int16_t _driveSignal )
    {
        driveSignal = _driveSignal;
    }

    virtual int16_t getMotorPWM()
    {
        return driveSignal;
    }


};

SpeedRegulator regulator;
MockSpeedometer speedometer;
MockDriver target;
float P = 4;
float D = 5;
float I = 6;
float filterLength = 7;

void setUp(void) {
    
    regulator.config( &speedometer, &target, P, I, D, filterLength );        
    regulator.setSetPoint( 10 );

    target.init();
    speedometer.init();
}

void testConfig()
{
    TEST_ASSERT_EQUAL( &speedometer, regulator.speedSource );
    TEST_ASSERT_EQUAL( &target, regulator.target );
    TEST_ASSERT_EQUAL_FLOAT( P, regulator.proportionalTerm );
    TEST_ASSERT_EQUAL_FLOAT( D, regulator.derivativeTerm );
    TEST_ASSERT_EQUAL_FLOAT( I, regulator.integratorTerm );
    TEST_ASSERT_EQUAL_FLOAT( filterLength, regulator.filterLength );
}

void testSetSetPoint()
{
    regulator.setSetPoint(10);

    TEST_ASSERT_EQUAL( 10, regulator.setPoint );
}

void testStartWithNormalLastInput()
{    
    regulator.start();
        
    TEST_ASSERT_TRUE( regulator.isOn );
    TEST_ASSERT_EQUAL( speedometer.getSpeedCPS(), regulator.input );
    TEST_ASSERT_EQUAL( regulator.input, regulator.lastInput );
    TEST_ASSERT_EQUAL( target.getMotorPWM(), regulator.integratorCumulativeValue );
}

void testStartWithLargeLastInput()
{
    target.driveSignal = 400; //A large value    
    regulator.start();
        
    TEST_ASSERT_TRUE( regulator.isOn );
    TEST_ASSERT_EQUAL( speedometer.getSpeedCPS(), regulator.input );
    TEST_ASSERT_EQUAL( regulator.input, regulator.lastInput );
    TEST_ASSERT_EQUAL( regulator.maxOutput, regulator.integratorCumulativeValue );
}

void testStop()
{    
    regulator.stop();
    TEST_ASSERT_FALSE( regulator.isOn );    
}

void testStepWhenStopped()
{
    regulator.stop();    
    regulator.run(0);
    TEST_ASSERT_EQUAL( 5, target.driveSignal ); //Should not change from starting value
}

void testStepWithSimpleProportionalTerm()
{
    regulator.integratorTerm = 0;
    regulator.derivativeTerm = 0;
    speedometer.speed = 3;
    TEST_ASSERT_EQUAL( 4, regulator.proportionalTerm );
    TEST_ASSERT_EQUAL( 10, regulator.setPoint );
    TEST_ASSERT_EQUAL( 3, speedometer.getSpeedCPS() );
    //Output = (10-3)*4 = 28;
    
    regulator.start();    
    regulator.integratorCumulativeValue = 0;

    regulator.run(0);
    TEST_ASSERT_EQUAL( 28, target.driveSignal );
}

void testStepWithSimpleProportionalTermAndLargeDiff()
{
    regulator.integratorTerm = 0;
    regulator.derivativeTerm = 0;
    regulator.proportionalTerm = 100;
    //P = 100
    //SetPoint16_t = 10;
    //Input = 3;
    // => Output = (10-3)*100 = 700;    
    regulator.start();    
    regulator.integratorCumulativeValue = 0;

    regulator.run(0);
    TEST_ASSERT_EQUAL( 255, target.driveSignal ); //Should not change from starting value
}

void testStepWithSimpleDerivativeTerm()
{
    regulator.integratorTerm = 0;
    regulator.proportionalTerm = 0;    
    TEST_ASSERT_EQUAL( 5, regulator.derivativeTerm );
    TEST_ASSERT_EQUAL( 10, regulator.setPoint );
    // => Output = (5-3)*5 = 10;
    
    regulator.start();    
    regulator.integratorCumulativeValue = 0;
    speedometer.speed = 5;

    regulator.run(0);
    TEST_ASSERT_EQUAL( 10, target.driveSignal ); 
}

void testStepWithSimpleIntegralTerm()
{
    regulator.derivativeTerm = 0;
    regulator.proportionalTerm = 0;    
    TEST_ASSERT_EQUAL( 6, regulator.integratorTerm );
    TEST_ASSERT_EQUAL( 3, speedometer.getSpeedCPS() );
    TEST_ASSERT_EQUAL( 10, regulator.setPoint );    
    // ITerm = 5 + 6*(10-3) = 47    
    regulator.start();    
    TEST_ASSERT_EQUAL( 5, regulator.integratorCumulativeValue );
    
    regulator.run(0);
    TEST_ASSERT_EQUAL( 47, target.driveSignal ); 
}

void testClampOutputHigh()
{
    TEST_ASSERT_EQUAL( regulator.maxOutput, regulator.clampOutput(regulator.maxOutput*2) );
}

void testClampOutputLow()
{
    TEST_ASSERT_EQUAL( 1, regulator.clampOutput(-regulator.maxOutput*2) );
}

void testClampOutputZero()
{
    TEST_ASSERT_EQUAL( 1, regulator.clampOutput(0) );
}

void testClampOutputOK()
{    
    TEST_ASSERT_EQUAL( regulator.maxOutput/2, regulator.clampOutput(regulator.maxOutput/2) );
}

void processMotorSpeedRegulator()
{    
    UNITY_BEGIN_INT();
    RUN_TEST( testConfig );    
    RUN_TEST( testStartWithLargeLastInput );
    RUN_TEST( testStartWithNormalLastInput );
    RUN_TEST( testStop );
    RUN_TEST( testSetSetPoint );
    RUN_TEST( testStepWhenStopped );
    RUN_TEST( testStepWithSimpleProportionalTerm );
    RUN_TEST( testStepWithSimpleProportionalTermAndLargeDiff );
    RUN_TEST( testStepWithSimpleDerivativeTerm );
    RUN_TEST( testStepWithSimpleIntegralTerm );
    RUN_TEST( testClampOutputHigh );
    RUN_TEST( testClampOutputLow );
    RUN_TEST( testClampOutputOK );
    RUN_TEST( testClampOutputZero );
    UNITY_END_INT();
}

