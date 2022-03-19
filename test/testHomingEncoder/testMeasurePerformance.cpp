#include <unityEx.h>
#include <HardwareClock.h>
#include <RotationalPosition.h>
#include <BasicEncoder.h>
#include <LinearPositionalEncoder.h>
#include <SpeedComputer.h>
#include <SerialStream.h>

#include "BasicEncoderListenerMock.h"

void testRotationalPositionGetClicksPerformance()
{    
#ifdef ARDUINO
    RotationalPosition sut ( 40000000 );
    HardwareClock hwClock;
    HardwareInterrupts hwInterrupts;

    uint32_t startTimeMicros = hwClock.getMicrosecondsSinceBoot();    
    volatile int32_t value = 0;
    for ( int16_t i = 0; i < 100; i++ )
    {
        value = sut.getClicks();
    }
    uint32_t endTimeMicros = hwClock.getMicrosecondsSinceBoot();    
    
    TEST_ASSERT_LESS_THAN_INT32( 20*1000L, endTimeMicros - startTimeMicros );
    TEST_ASSERT_GREATER_THAN_INT32( 15*1000L, endTimeMicros - startTimeMicros );

#else
    TEST_IGNORE_MESSAGE( "No performance measurements on Native" );
#endif
}

void testBasicEncoderWithMockListenersPerformance()
{    
#ifdef ARDUINO
    HardwarePinsMock hwPins;
    HardwareInterrupts hwInterrupts;
    HardwareClock hwClock;
    BasicEncoder* encoder =  BasicEncoderFactory::config<0>( 1, 2, 3, &hwPins );
    BasicEncoderListenerMock listenerMock1;
    encoder->addListener( &listenerMock1 );    
    //hwInterrupts.enableInterrupts();

    uint32_t startTimeMicros = hwClock.getMicrosecondsSinceBoot();
    for ( int16_t i = 0; i < 10000; i++ ) //Max number of clicks per second
    {
        BasicEncoderFactory::isr_encoder<0>();        
    }
    uint32_t endTimeMicros = hwClock.getMicrosecondsSinceBoot();
    
    TEST_ASSERT_LESS_THAN_INT32( 70*1000L, endTimeMicros - startTimeMicros );    
    TEST_ASSERT_GREATER_THAN_INT32( 65*1000L, endTimeMicros - startTimeMicros );    

    TEST_ASSERT_EQUAL( 10000, listenerMock1.stepCounter ); //To ensure the loop isn't optimized away.    
#else
    TEST_IGNORE_MESSAGE( "No performance measurements on Native" );
#endif
}

void testBasicEncoderWithLinearAndSpeedMeasurementPerformance()
{    
#ifdef ARDUINO
    HardwarePinsMock hwPins;
    HardwareInterrupts hwInterrupts;
    HardwareClock hwClock;
    BasicEncoder* encoder =  BasicEncoderFactory::config<0>( 1, 2, 3, &hwPins );
    LinearPositionEncoder linear( &hwInterrupts );    
    linear.signalHomingISR(); //To ensure we are homed before starting.
    SpeedComputer speed( &hwClock, &hwInterrupts );
    encoder->addListener( &linear );
    encoder->addListener ( &speed );    
    
    uint32_t startTimeMicros = hwClock.getMicrosecondsSinceBoot();
    for ( int16_t i = 0; i < 10000; i++ ) //Max clicks per second
    {
        BasicEncoderFactory::isr_encoder<0>();        
    }    
    uint32_t endTimeMicros = hwClock.getMicrosecondsSinceBoot();    

    TEST_ASSERT_LESS_THAN_INT32( 120*1000L, endTimeMicros - startTimeMicros );    
    TEST_ASSERT_GREATER_THAN_INT32( 110*1000L, endTimeMicros - startTimeMicros );    

    TEST_ASSERT_EQUAL( 10000, linear.getLinearPosition() ); //To ensure the loop isn't optimized away.    
#else
    TEST_IGNORE_MESSAGE( "No performance measurements on Native" );
#endif
}

void runAllTestMeasurePerformance()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testRotationalPositionGetClicksPerformance );
    RUN_TEST( testBasicEncoderWithMockListenersPerformance ) ;
    RUN_TEST( testBasicEncoderWithLinearAndSpeedMeasurementPerformance );    
    UNITY_END_INT();
}