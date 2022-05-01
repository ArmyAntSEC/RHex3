#include <unityEx.h>
#define private public
#include <LegCommandSender.h>

struct I2cTextSenderMock: public I2cTextSenderInterface
{
    int8_t lastAddress;
    int32_t buffer[16];
    virtual void sendText( int8_t i2cAddress, int32_t const * data, int8_t len )
    {
        lastAddress = i2cAddress;        
        memcpy( buffer, data, len*sizeof(data) );                        
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

    TEST_ASSERT_EQUAL( 10, rawSender.lastAddress );

    int32_t expected[] = {1, 1234, 4567 };
    TEST_ASSERT_EQUAL_INT32_ARRAY ( expected, rawSender.buffer, 3 );    
    
}

void runAllTestsLegCommandSender()
{
    UNITY_BEGIN_INT();
    RUN_TEST( createLegCommandSender );
    RUN_TEST( testSendLegCommand );
    UNITY_END_INT();
}