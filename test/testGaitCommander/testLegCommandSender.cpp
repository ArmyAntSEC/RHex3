#include <unityEx.h>
#define private public
#include <LegCommandSender.h>

struct I2cTextSenderMock: public I2cTextSenderInterface
{
    int8_t lastAddress;
    char buffer[128];
    virtual void sendText( int8_t i2cAddress, char * str, int8_t len )
    {
        lastAddress = i2cAddress;
        strncpy( buffer, str, len );
        buffer[127] = 0;
    }
};

void createLegCommandSender()
{
    I2cTextSenderMock rawSender;
    
    LegCommandSender sut(&rawSender);
    
    TEST_ASSERT_EQUAL( &rawSender, sut.rawSender );
}

void testSendLegCommand()
{
    I2cTextSenderMock rawSender;        
    MotorCommanderGoal goal( 1234, 4567 );

    LegCommandSender sut(&rawSender);   
    
    sut.sendCommand( LegIdentifiers::BACK_RIGHT, goal );

    TEST_ASSERT_EQUAL_STRING ( 
        "{\"motorID\":1,\"targetPositionClicks\":1234,\"targetRelativeTimeMicros\":4567}",
        rawSender.buffer );    
}

void runAllTestsLegCommandSender()
{
    UNITY_BEGIN_INT();
    RUN_TEST( createLegCommandSender );
    RUN_TEST( testSendLegCommand );
    UNITY_END_INT();
}