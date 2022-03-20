#include <unityEx.h>

#define private public
#include <LinearPositionalEncoder.h>

void testShouldStepForward()
{
    HardwareInterruptsMock hwInterrupts;    
    LinearPositionEncoder sut(&hwInterrupts);    
    TEST_ASSERT_EQUAL( 0, sut.getLinearPosition() );

    sut.signalStepForwardISR();

    TEST_ASSERT_EQUAL( 1, sut.getLinearPosition() );
}

void testShouldHomeIfNotHomed()
{
    HardwareInterruptsMock hwInterrupts;    
    LinearPositionEncoder sut(&hwInterrupts);    

    sut.linearPosition = 100;

    sut.signalHomingISR();

    TEST_ASSERT_EQUAL( 0, sut.getLinearPosition() );
    TEST_ASSERT_TRUE( sut.isHomed() );
}

void testShouldNotHomeIfHomed()
{
    HardwareInterruptsMock hwInterrupts;    
    LinearPositionEncoder sut(&hwInterrupts);    

    sut.linearPosition = 100;
    sut.encoderIsHomed = true;

    sut.signalHomingISR();

    TEST_ASSERT_EQUAL( 100, sut.getLinearPosition() );
}

void testShouldRestoreInterruptFlags()
{
    HardwareInterruptsMock hwInterrupts;    
    LinearPositionEncoder sut(&hwInterrupts);    

    sut.getLinearPosition();

    TEST_ASSERT_EQUAL( 1, hwInterrupts.interruptsEnabledCount );
    TEST_ASSERT_EQUAL( 1, hwInterrupts.interruptsDisabledCount );     
}

void runAllTestsLinearPositionEncoder()
{
    UNITY_BEGIN_INT();
    RUN_TEST( testShouldStepForward );   
    RUN_TEST( testShouldHomeIfNotHomed ); 
    RUN_TEST( testShouldNotHomeIfHomed ); 
    RUN_TEST( testShouldRestoreInterruptFlags );
    UNITY_END_INT();
}