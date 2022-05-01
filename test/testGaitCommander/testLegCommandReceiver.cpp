#include <unityEx.h>
#define private public
#include <LegCommandReceiver.h>

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


void testCreateI2CReceiver()
{
    I2CReceiverWrappeMock i2cMock;

    I2CReceiver& sut = I2CReceiver::getSingletonInstance();
    I2CReceiver::configI2CPort( &i2cMock );

    TEST_ASSERT_EQUAL( sut.onReceive, i2cMock.onReceive );
}

void testI2COnReceive()
{    
    I2CReceiverWrappeMock i2cMock;
    int32_t myBuffer[] = {1234, 4567, 5676, 4567};
    memcpy( i2cMock.buffer, myBuffer, sizeof(myBuffer) );

    I2CReceiver& sut = I2CReceiver::getSingletonInstance();
    sut.onReceive( sizeof(myBuffer) );

    TEST_ASSERT_EQUAL_INT32_ARRAY( myBuffer, sut.messageBuffer, 4 );
}

void testI2COnReceiveWrongBytes()
{
    I2CReceiverWrappeMock i2cMock;
    int32_t myBuffer[] = {1234, 4567, 5676};
    memcpy( i2cMock.buffer, myBuffer, sizeof(myBuffer) );

    I2CReceiver& sut = I2CReceiver::getSingletonInstance();
    sut.onReceive( sizeof(myBuffer) );

    TEST_ASSERT_EACH_EQUAL_INT32( 0, sut.messageBuffer, 4 );

}


void runAllTestsLegCommandReceiver()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testCreateI2CReceiver );    
    RUN_TEST( testI2COnReceive );
    RUN_TEST( testI2COnReceiveWrongBytes );
    UNITY_END_INT();
}