#include <unity.h>
#define private public

#include <BasicEncoder.h>

class BasicEncoderListenerMock: public BasicEncoderListener
{
    public:

    int stepCounter = 0;
    int homingCounter = 0;

    void signalStepForwardISR() 
    {
        stepCounter++;
    }
    
    void signalHomingISR() 
    {
        homingCounter++;
    }
};

void testBasicEncoderFactoryConfig()
{
    int encoderPin1 = 0;
    int encoderPin2 = 1;
    int homingPin = 2;
    BasicEncoderListenerMock listener;
    BasicEncoder* sut = BasicEncoderFactory::config<0>( encoderPin1, encoderPin2, homingPin, &listener );

    TEST_ASSERT_EQUAL(encoderPin1, sut->encoderPin1 );
    TEST_ASSERT_EQUAL(encoderPin2, sut->encoderPin2 );
    TEST_ASSERT_EQUAL(homingPin, sut->homingPin );
    TEST_ASSERT_EQUAL( &listener, sut->listener );

    TEST_ASSERT_EQUAL( &BasicEncoderFactory::stateList[0], sut );    
    TEST_ASSERT_EQUAL( BasicEncoderFactory::isr_encoder<0>, HardwareInterface::isrList[encoderPin1] );
    TEST_ASSERT_EQUAL( BasicEncoderFactory::isr_homing<0>, HardwareInterface::isrList[homingPin] );
}

void testBasicEncoderSignalStepForwardISR()
{
    BasicEncoderListenerMock listener;
    BasicEncoder* sut = BasicEncoderFactory::config<0>( 0, 1, 2, &listener );
    TEST_ASSERT_EQUAL( 0, listener.stepCounter );
    
    sut->signalStepForwardISR();

    TEST_ASSERT_EQUAL( 1, listener.stepCounter );
}

void testBasicEncoderSignalHomingISR()
{
    BasicEncoderListenerMock listener;
    BasicEncoder* sut = BasicEncoderFactory::config<0>( 0, 1, 2, &listener );
    TEST_ASSERT_EQUAL( 0, listener.homingCounter );
    
    sut->signalHomingISR();

    TEST_ASSERT_EQUAL( 1, listener.homingCounter );
}

void runAllTestsBasicEncoder()
{            
    RUN_TEST( testBasicEncoderFactoryConfig );
    RUN_TEST( testBasicEncoderSignalStepForwardISR );
    RUN_TEST( testBasicEncoderSignalHomingISR );
}