#include <unity.h>

#include <HomingEncoder.h>

void testDoOverflow()
{
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


void runAllTestsHomingEncoderOverflow()
{        
    RUN_TEST( testDoOverflow );    
}