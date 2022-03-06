#include <unityEx.h>
#include <HardwareClock.h>
#include <RotationalPosition.h>
#include <BasicEncoder.h>
#include <LinearPositionalEncoder.h>
#include <SpeedComputer.h>
#include <SerialStream.h>

struct BasicEncoderListenerMock: public BasicEncoderListener
{
    volatile int stepCounter;
    volatile int homingCounter;

    virtual void signalStepForwardISR()
    {
        stepCounter++;    
    }

    virtual void signalHomingISR()
    {
        homingCounter++;
    }
};

void testRotationalPositionGetClicksPerformance()
{    
#ifdef ARDUINO
    RotationalPosition sut ( 40000000 );
    HardwareClock hwClock;
    HardwareInterrupts hwInterrupts;
    //hwInterrupts.enableInterrupts();

    unsigned long startTimeMicros = hwClock.getMicrosecondsSinceBoot();    
    volatile long value = 0;
    for ( int i = 0; i < 100; i++ )
    {
        value = sut.getClicks();
    }
    unsigned long endTimeMicros = hwClock.getMicrosecondsSinceBoot();    
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

    unsigned long startTimeMicros = hwClock.getMicrosecondsSinceBoot();
    for ( int i = 0; i < 100; i++ )
    {
        BasicEncoderFactory::isr_encoder<0>();
        BasicEncoderFactory::isr_homing<0>();                
        volatile int stepCounter = listenerMock1.stepCounter;
        volatile int homingCounter = listenerMock1.homingCounter;
    }

    unsigned long endTimeMicros = hwClock.getMicrosecondsSinceBoot();
    TEST_ASSERT_LESS_THAN_INT32( 2000, endTimeMicros - startTimeMicros );    
    TEST_ASSERT_GREATER_THAN_INT32( 1000, endTimeMicros - startTimeMicros );    
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
    LinearPositionEncoder linear ( &hwInterrupts );
    SpeedComputer speed ( &hwClock, &hwInterrupts );
    encoder->addListener( &linear );
    encoder->addListener ( &speed );
    //hwInterrupts.enableInterrupts();
    
    unsigned long startTimeMicros = hwClock.getMicrosecondsSinceBoot();
    for ( int i = 0; i < 6000; i++ ) //Max clicks per second
    {
        BasicEncoderFactory::isr_encoder<0>();
        BasicEncoderFactory::isr_homing<0>();        
        volatile long linPos = linear.getLinearPosition();
        volatile long speedCPS = speed.getSpeedCPS();
    }
    unsigned long endTimeMicros = hwClock.getMicrosecondsSinceBoot();
    TEST_ASSERT_LESS_THAN_INT32( 500*1000L, endTimeMicros - startTimeMicros );    
    TEST_ASSERT_GREATER_THAN_INT32( 300*1000L, endTimeMicros - startTimeMicros );    
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