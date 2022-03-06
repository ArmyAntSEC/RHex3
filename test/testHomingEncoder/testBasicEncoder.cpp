#include <unityEx.h>
#define private public

#include "BasicEncoderListenerMock.h"

void testBasicEncoderFactoryConfig()
{
    int encoderPin1 = 0;
    int encoderPin2 = 1;
    int homingPin = 2;
    BasicEncoderListenerMock listener;
    HardwarePinsMock pins;    
    BasicEncoder* sut = BasicEncoderFactory::config<0>( encoderPin1, encoderPin2, homingPin, &pins );
    sut->addListener( &listener );

    TEST_ASSERT_EQUAL(encoderPin1, sut->encoderPin1 );
    TEST_ASSERT_EQUAL(encoderPin2, sut->encoderPin2 );
    TEST_ASSERT_EQUAL(homingPin, sut->homingPin );    

    TEST_ASSERT_EQUAL( &BasicEncoderFactory::stateList[0], sut );    
    TEST_ASSERT_EQUAL( BasicEncoderFactory::isr_encoder<0>, pins.isrList[encoderPin1] );
    TEST_ASSERT_EQUAL(  pins.CHANGE, pins.pinStatuses[encoderPin1] );
    TEST_ASSERT_EQUAL( BasicEncoderFactory::isr_homing<0>, pins.isrList[homingPin] );    
    TEST_ASSERT_EQUAL(  pins.FALLING, pins.pinStatuses[homingPin] );
    
    TEST_ASSERT_EQUAL( pins.INPUT_PULLUP, pins.pinModes[encoderPin1] );
    TEST_ASSERT_EQUAL( pins.INPUT_PULLUP, pins.pinModes[encoderPin2] );
    TEST_ASSERT_EQUAL( pins.INPUT_PULLUP, pins.pinModes[homingPin] );
}

void testAddTwoListeners()
{      
    HardwarePinsMock pins;    
    BasicEncoder* sut = BasicEncoderFactory::config<0>( 0, 1, 2, &pins );

    BasicEncoderListener* listener1 = (BasicEncoderListener*)1234;
    BasicEncoderListener* listener2 = (BasicEncoderListener*)5678;
    sut->addListener( listener1 );
    sut->addListener( listener2 );

    TEST_ASSERT_EQUAL( listener1, sut->listeners[0] );
    TEST_ASSERT_EQUAL( listener2, sut->listeners[1] );
}

void testShouldCallBothListeners()
{      
    HardwarePinsMock pins;    
    BasicEncoder* sut = BasicEncoderFactory::config<0>( 0, 1, 2, &pins );

    BasicEncoderListenerMock listener1;
    BasicEncoderListenerMock listener2;
    sut->addListener( &listener1 );
    sut->addListener( &listener2 );

    sut->signalHomingISR();
    sut->signalStepForwardISR();

    TEST_ASSERT_EQUAL( 1, listener1.homingCounter );
    TEST_ASSERT_EQUAL( 1, listener1.stepCounter );
    TEST_ASSERT_EQUAL( 1, listener2.homingCounter );
    TEST_ASSERT_EQUAL( 1, listener2.stepCounter );

}

void testBasicEncoderFactorySignalStepForwardISR()
{
    BasicEncoderListenerMock listener;
    HardwarePinsMock pins;    
    BasicEncoder* sut = BasicEncoderFactory::config<0>( 0, 1, 2, &pins );
    sut->addListener( &listener );
    TEST_ASSERT_EQUAL( 0, listener.stepCounter );
    
    BasicEncoderFactory::isr_encoder<0>();

    TEST_ASSERT_EQUAL( 1, listener.stepCounter );
}

void testBasicEncoderFactorySignalHomingISR()
{
    BasicEncoderListenerMock listener;
    HardwarePinsMock pins;    
    BasicEncoder* sut = BasicEncoderFactory::config<0>( 0, 1, 2, &pins );
    sut->addListener( &listener );
    
    TEST_ASSERT_EQUAL( 0, listener.homingCounter );
    
    BasicEncoderFactory::isr_homing<0>();

    TEST_ASSERT_EQUAL( 1, listener.homingCounter );
}

void runAllTestsBasicEncoder()
{               
    UNITY_BEGIN_INT(); 
    RUN_TEST( testBasicEncoderFactoryConfig );
    RUN_TEST( testAddTwoListeners );
    RUN_TEST( testShouldCallBothListeners );
    RUN_TEST( testBasicEncoderFactorySignalStepForwardISR );
    RUN_TEST( testBasicEncoderFactorySignalHomingISR );    
    UNITY_END_INT();
}
