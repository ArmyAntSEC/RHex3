#include <unity.h>

#include <HomingEncoder.h>

void testStepEncoder()
{
    HomingEncoder* encoder = HomingEncoderFactory::config<0>(0,0,0,0);    

    HomingEncoderFactory::isr_encoder<0>();
    TEST_ASSERT_EQUAL( 1, encoder->getRawPos() );
}

void testStepHoming()
{    
    HomingEncoder* encoder = HomingEncoderFactory::config<0>(0,0,0,0);
    TEST_ASSERT_FALSE( encoder->isHomed() );

    //First step the homing once adn make sure the debouncer catches that.
    HomingEncoderFactory::isr_homing<0>();
    TEST_ASSERT_FALSE( encoder->isHomed() );

    //Step the encoder 200+ times to pass the debouncer
    for ( int i = 0; i < 205; i++ ) 
    {
        HomingEncoderFactory::isr_encoder<0>();
    }

    //Step the homing to home.
    HomingEncoderFactory::isr_homing<0>();
    TEST_ASSERT_TRUE( encoder->isHomed() );

}


void runAllTestsHomingEncoderISR()
{        
    UNITY_BEGIN();
    RUN_TEST( testStepEncoder );
    RUN_TEST( testStepHoming );
    UNITY_END();
}