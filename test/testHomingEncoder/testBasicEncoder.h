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
    HardwarePins pins;    
    BasicEncoder* sut = BasicEncoderFactory::config<0>( encoderPin1, encoderPin2, homingPin, &listener, &pins );

    TEST_ASSERT_EQUAL(encoderPin1, sut->encoderPin1 );
    TEST_ASSERT_EQUAL(encoderPin2, sut->encoderPin2 );
    TEST_ASSERT_EQUAL(homingPin, sut->homingPin );
    TEST_ASSERT_EQUAL( &listener, sut->listener );

    TEST_ASSERT_EQUAL( &BasicEncoderFactory::stateList[0], sut );    
    TEST_ASSERT_EQUAL( BasicEncoderFactory::isr_encoder<0>, pins.isrList[encoderPin1] );
    TEST_ASSERT_EQUAL(  pins.CHANGE, pins.pinStatuses[encoderPin1] );
    TEST_ASSERT_EQUAL( BasicEncoderFactory::isr_homing<0>, pins.isrList[homingPin] );    
    TEST_ASSERT_EQUAL(  pins.FALLING, pins.pinStatuses[homingPin] );
    
    TEST_ASSERT_EQUAL( pins.INPUT_PULLUP, pins.pinModes[encoderPin1] );
    TEST_ASSERT_EQUAL( pins.INPUT_PULLUP, pins.pinModes[encoderPin2] );
    TEST_ASSERT_EQUAL( pins.INPUT_PULLUP, pins.pinModes[homingPin] );
}

void testBasicEncoderFactorySignalStepForwardISR()
{
    BasicEncoderListenerMock listener;
    HardwarePins pins;    
    BasicEncoder* sut = BasicEncoderFactory::config<0>( 0, 1, 2, &listener, &pins );
    TEST_ASSERT_EQUAL( 0, listener.stepCounter );
    
    BasicEncoderFactory::isr_encoder<0>();

    TEST_ASSERT_EQUAL( 1, listener.stepCounter );
}

void testBasicEncoderFactorySignalHomingISR()
{
    BasicEncoderListenerMock listener;
    HardwarePins pins;    
    BasicEncoder* sut = BasicEncoderFactory::config<0>( 0, 1, 2, &listener, &pins );
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
