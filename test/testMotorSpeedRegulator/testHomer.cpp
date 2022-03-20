#include <unityEx.h>

#include <MotorSpeedRegulatorInterfaces.h>
#include <HomingEncoderInterfaces.h>

#define private public
#include <Homer.h>

struct LinPosMock: public LinearPositionProvider
{
    int32_t currPos = 0;
    bool encoderIsHomed = false;
    
    virtual int32_t getLinearPosition()
    {
        return currPos;
    }

    virtual bool isHomed()
    {
        return encoderIsHomed;
    }
};

struct MotorDriverMock: public MotorDriverInterface
{
    int16_t motorPWM = 0;

    virtual void setMotorPWM( int16_t _motorPWM )
    {
        motorPWM = _motorPWM;
    }

    virtual int16_t getMotorPWM()
    {
        return motorPWM;
    }
};

void testDoSimpleHoming()
{
    MotorDriverMock driver;
    LinPosMock linPos;

    Homer sut( &driver, &linPos );

    sut.start();

    TEST_ASSERT_EQUAL( 64, driver.getMotorPWM() );
    
    sut.run( 1234 );
    TEST_ASSERT_EQUAL( 64, driver.getMotorPWM() );
    
    linPos.encoderIsHomed = true;
    sut.run( 2345 );
    TEST_ASSERT_EQUAL( 0, driver.getMotorPWM() );
    
    //Now verify that once homed, Homer will not issue more orders.
    driver.setMotorPWM(128);
    sut.run( 3456 );
    TEST_ASSERT_EQUAL( 128, driver.getMotorPWM() );
}

void testEnsureHomingDoesNotHappenBeforeBeingStarted()
{
    MotorDriverMock driver;
    LinPosMock linPos;
    Homer sut( &driver, &linPos );

    driver.setMotorPWM(128);

    linPos.encoderIsHomed = true;
    sut.run( 2345 );
    TEST_ASSERT_EQUAL( 128, driver.getMotorPWM() );
}

void runAllTestsHomer()
{
    UNITY_BEGIN_INT();
    RUN_TEST(testDoSimpleHoming);
    RUN_TEST( testEnsureHomingDoesNotHappenBeforeBeingStarted );
    UNITY_BEGIN_INT();
}