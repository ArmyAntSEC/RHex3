#include <unity.h>
#define private public
#include <MotorSpeedCommander.h>
#include <HomingEncoder.h>

void testConfig()
{
    HomingEncoder* encoder = (HomingEncoder*)123;
    MotorSpeedRegulator* regulator = (MotorSpeedRegulator*)456;

    MotorSpeedCommander commander;
    commander.encoder = 0;
    commander.regulator = 0;

    commander.config( encoder, regulator );

    TEST_ASSERT_EQUAL( encoder, commander.encoder );
    TEST_ASSERT_EQUAL( regulator, commander.regulator );
}

void testInit()
{
    
}

/*
computeTargetSpeedTakingNegativeTimeIntoAccount
capTargetSpeed
computeTargetSpeed
run
hasArrived
stop
*/

void processMotorSpeedCommander()
{
    RUN_TEST( testConfig );
    RUN_TEST( testInit );
}