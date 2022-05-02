#include <unityEx.h>
#define private public
#include <LegCommandReceiver.h>
#include <MotorSpeedRegulatorInterfaces.h>

struct I2CReceiverWrappeMock: public I2CReceiverWrapperInterface
{    
    void (*onReceive)(int32_t numBytes);

    uint8_t buffer[16];
    int8_t bufferPointer = 0;

    virtual void setOnReceive( void (*_onReceive)(int32_t numBytes) )
    {
        onReceive = _onReceive;
    }

    virtual uint8_t read()
    {
        return buffer[bufferPointer++];
    }
};

struct MotorSpeedCommanderMock: public MotorSpeedCommanderInterface
{
    MotorCommanderGoal lastGoal;
    int32_t lastTime = 0;

    virtual void setGoal(MotorCommanderGoal command, int32_t nowMicros )
    {
        lastGoal = command;
        lastTime = nowMicros;
    }
};


void testCreateLegCommandReceiver()
{
    I2CReceiverWrappeMock i2cMock;
    MotorSpeedCommanderMock commander1;
    MotorSpeedCommanderMock commander2;
    HardwareClockMock hwClock;

    LegCommandReceiver& sut = LegCommandReceiver::getSingletonInstance();
    sut.config( &i2cMock, &commander1, &commander2, &hwClock );

    TEST_ASSERT_EQUAL( sut.onReceive, i2cMock.onReceive );
}

void testLegCommandOnReceive()
{    
    I2CReceiverWrappeMock i2cMock;
    MotorSpeedCommanderMock commander1;
    MotorSpeedCommanderMock commander2;
    HardwareClockMock hwClock;
    hwClock.microsSinceStart = 7891;

    int32_t myBuffer[] = {1, 4567, 5676};
    memcpy( i2cMock.buffer, myBuffer, sizeof(myBuffer) );

    LegCommandReceiver& sut = LegCommandReceiver::getSingletonInstance();    
    sut.config( &i2cMock, &commander1, &commander2, &hwClock );
    
    sut.onReceive( sizeof(myBuffer) );    

    TEST_ASSERT_EQUAL_INT32_ARRAY( myBuffer, sut.messageBuffer, 3 );
 
    TEST_ASSERT_EQUAL_INT( 0, commander1.lastGoal.targetPositionClicks );
    TEST_ASSERT_EQUAL_INT( 0, commander1.lastGoal.targetRelativeTimeMicros );
    TEST_ASSERT_EQUAL_INT( 0, commander1.lastTime );
    
    TEST_ASSERT_EQUAL_INT( 4567, commander2.lastGoal.targetPositionClicks );
    TEST_ASSERT_EQUAL_INT( 5676, commander2.lastGoal.targetRelativeTimeMicros );
    TEST_ASSERT_EQUAL_INT( 7891, commander2.lastTime );
}

void testLegCommandOnReceiveOtherLeg()
{    
    I2CReceiverWrappeMock i2cMock;
    MotorSpeedCommanderMock commander1;
    MotorSpeedCommanderMock commander2;
    HardwareClockMock hwClock;
    hwClock.microsSinceStart = 7823;

    int32_t myBuffer[] = {0, 4567, 5676};
    memcpy( i2cMock.buffer, myBuffer, sizeof(myBuffer) );

    LegCommandReceiver& sut = LegCommandReceiver::getSingletonInstance();    
    sut.config( &i2cMock, &commander1, &commander2, &hwClock );
    
    sut.onReceive( sizeof(myBuffer) );    

    TEST_ASSERT_EQUAL_INT32_ARRAY( myBuffer, sut.messageBuffer, 3 );
 
    TEST_ASSERT_EQUAL_INT( 4567, commander1.lastGoal.targetPositionClicks );
    TEST_ASSERT_EQUAL_INT( 5676, commander1.lastGoal.targetRelativeTimeMicros );
    TEST_ASSERT_EQUAL_INT( 7823, commander1.lastTime );

    TEST_ASSERT_EQUAL_INT( 0, commander2.lastGoal.targetPositionClicks );
    TEST_ASSERT_EQUAL_INT( 0, commander2.lastGoal.targetRelativeTimeMicros );
    TEST_ASSERT_EQUAL_INT( 0, commander2.lastTime );

}

void testLegCommandOnReceiveWrongBytes()
{
    I2CReceiverWrappeMock i2cMock;
    MotorSpeedCommanderMock commander1;
    MotorSpeedCommanderMock commander2;
    HardwareClockMock hwClock;

    int32_t myBuffer[] = {1234, 4567, 5676, 4321};
    memcpy( i2cMock.buffer, myBuffer, sizeof(myBuffer) );

    LegCommandReceiver& sut = LegCommandReceiver::getSingletonInstance();
    sut.config( &i2cMock, &commander1, &commander2, &hwClock );
    
    sut.onReceive( sizeof(myBuffer) );

    TEST_ASSERT_EACH_EQUAL_INT32( 0, sut.messageBuffer, 3 );

    TEST_ASSERT_EQUAL_INT( 0, commander1.lastGoal.targetPositionClicks );
    TEST_ASSERT_EQUAL_INT( 0, commander1.lastGoal.targetRelativeTimeMicros );
    TEST_ASSERT_EQUAL_INT( 0, commander1.lastTime );

    TEST_ASSERT_EQUAL_INT( 0, commander2.lastGoal.targetPositionClicks );
    TEST_ASSERT_EQUAL_INT( 0, commander2.lastGoal.targetRelativeTimeMicros );
    TEST_ASSERT_EQUAL_INT( 0, commander2.lastTime );
}


void runAllTestsLegCommandReceiver()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testCreateLegCommandReceiver );    
    RUN_TEST( testLegCommandOnReceive );
    RUN_TEST( testLegCommandOnReceiveOtherLeg );
    RUN_TEST( testLegCommandOnReceiveWrongBytes );
    UNITY_END_INT();
}