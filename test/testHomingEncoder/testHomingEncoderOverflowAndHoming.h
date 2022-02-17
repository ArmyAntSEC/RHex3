#include <unity.h>

#include <HomingEncoder.h>

void testDoOverflow()
{
    TEST_IGNORE();
    HomingEncoder* encoder = HomingEncoderFactory::config<0>(0,0,0,0);    

    //Step the encoder 2000 times
    for ( int i = 0; i < 2000; i++ ) {
        HomingEncoderFactory::isr_encoder<0>();
    }

    //Now run the overflow logic
    encoder->handleOverflow();

    TEST_ASSERT_EQUAL( 1, encoder->getPosition().getLaps() );
    TEST_ASSERT_EQUAL( 2000-1796, encoder->getPosition().getClickPosition() );         
    TEST_ASSERT_INT_WITHIN( 100, 1e6-962666, encoder->getPosition().getRemainderMicroClicks() );    
}

void testDoOverflowRecursive()
{
    TEST_IGNORE();
    HomingEncoder* encoder = HomingEncoderFactory::config<0>(0,0,0,0);    

    //Step the encoder 4000 times
    for ( int i = 0; i < 4000; i++ ) {
        HomingEncoderFactory::isr_encoder<0>();
    }

    //Now run the overflow logic
    encoder->handleOverflow();

    TEST_ASSERT_EQUAL( 2, encoder->getPosition().getLaps() );
    TEST_ASSERT_EQUAL( 4000-2*1796, encoder->getPosition().getClickPosition() );         
    TEST_ASSERT_INT_WITHIN( 100, 2*(1e6-962666), encoder->getPosition().getRemainderMicroClicks() );    
}

void doTestForceHomed()
{
    HomingEncoder* encoder = HomingEncoderFactory::config<0>(0,0,0,0);        

    //Step the encoder 2000 times
    for ( int i = 0; i < 2000; i++ ) {
        HomingEncoderFactory::isr_encoder<0>();
    }
    encoder->handleOverflow();
    
    //Do the forced homing.
    encoder->forceHomed();
    
    TEST_ASSERT_EQUAL( 2000-1796, encoder->getPosAtLastHome().getClickPosition() );
    TEST_ASSERT_EQUAL( 1, encoder->getPosAtLastHome().getLaps() );

    TEST_ASSERT_EQUAL( 0, encoder->getPosition().getClickPosition() );
    TEST_ASSERT_EQUAL( 0, encoder->getPosition().getLaps() );
    TEST_ASSERT_EQUAL( 0, encoder->getPosition().getRemainderMicroClicks() );

}

void runAllTestsHomingEncoderOverflowAndHoming()
{        
    UNITY_BEGIN();
    RUN_TEST( testDoOverflow );   
    RUN_TEST( testDoOverflowRecursive ); 
    RUN_TEST( doTestForceHomed );
    UNITY_END();
}