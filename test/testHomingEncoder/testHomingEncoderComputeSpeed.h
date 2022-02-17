#include <unity.h>

#include <HomingEncoder.h>


void doTestComputeSpeed()
{
    TEST_IGNORE();
 
    HomingEncoder* encoder = HomingEncoderFactory::config<0>(0,0,0,0);        

    //This function causes one millisecond to pass when run on native.
    HardwareInterface::getMillisecondsSinceBoot();

    //Step once to prime with some data.
    HomingEncoderFactory::isr_encoder<0>();

    //Run once to get a baseline
    encoder->run();

    TEST_ASSERT_EQUAL( 1, encoder->positionAtLastSpeedMeasurement.getClickPosition() );
    TEST_ASSERT_EQUAL( 10010, encoder->timestampMicrosAtLastSpeedMeasurement );
    TEST_ASSERT_INT_WITHIN( 1, 100, encoder->speed_cps );

    //Step once more to get some more data.
    HomingEncoderFactory::isr_encoder<0>();
    HardwareInterface::getMillisecondsSinceBoot();

    //Run once more to get a new speed.
    encoder->run();

    TEST_ASSERT_EQUAL( 2, encoder->positionAtLastSpeedMeasurement.getClickPosition() );
    TEST_ASSERT_EQUAL( 20020, encoder->timestampMicrosAtLastSpeedMeasurement );
    TEST_ASSERT_INT_WITHIN( 1, 100, encoder->speed_cps );
}

void runAllTestsHomingEncoderComputeSpeed()
{            
    RUN_TEST( doTestComputeSpeed );
}