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

    TEST_FAIL_MESSAGE( "Need to swich over to the HardwarePin class to interface with HW." );
}

void testBasicEncoderFactorySignalStepForwardISR()
{
    BasicEncoderListenerMock listener;
    BasicEncoder* sut = BasicEncoderFactory::config<0>( 0, 1, 2, &listener );
    TEST_ASSERT_EQUAL( 0, listener.stepCounter );
    
    BasicEncoderFactory::isr_encoder<0>();

    TEST_ASSERT_EQUAL( 1, listener.stepCounter );
}

void testBasicEncoderFactorySignalHomingISR()
{
    BasicEncoderListenerMock listener;
    BasicEncoder* sut = BasicEncoderFactory::config<0>( 0, 1, 2, &listener );
    TEST_ASSERT_EQUAL( 0, listener.homingCounter );
    
    BasicEncoderFactory::isr_homing<0>();

    TEST_ASSERT_EQUAL( 1, listener.homingCounter );
}

void runAllTestsBasicEncoder()
{            
    UNITY_BEGIN();
    RUN_TEST( testBasicEncoderFactoryConfig );
    RUN_TEST( testBasicEncoderFactorySignalStepForwardISR );
    RUN_TEST( testBasicEncoderFactorySignalHomingISR );
    UNITY_END();
}
