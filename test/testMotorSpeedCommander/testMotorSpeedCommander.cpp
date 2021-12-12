#include <unity.h>
#define private public
#include <MotorSpeedCommander.h>
#include <MockHomingEncoder.h>
#include <TestingMotorSpeedRegulator.h>

void testConfig()
{
    HomingEncoderInterface* encoder = (HomingEncoderInterface*)123;
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
    MotorSpeedCommander commander;
    MockHomingEncoder encoder;
    TestingMotorSpeedRegulator regulator;
    commander.config( &encoder, &regulator );
    
    int timeToMoveTo = 100;
    int posToMoveTo = 200;

    commander.init( timeToMoveTo, posToMoveTo );

    TEST_ASSERT_EQUAL( 0, commander.posToMoveTo.getLaps() );
    TEST_ASSERT_EQUAL( posToMoveTo, commander.posToMoveTo.getClickPosition() );
    TEST_ASSERT_EQUAL( 0, commander.posToMoveTo.getRemainder() );

    TEST_ASSERT_EQUAL( timeToMoveTo, commander.timeToArrive );
    TEST_ASSERT_EQUAL( false, commander.hasArrived() );
    TEST_ASSERT_EQUAL( 1, regulator.countInit );
    TEST_ASSERT_EQUAL( true, regulator.hardBreakMode );
}

void testComputeTargetSpeedTakingNegativeTimeIntoAccountNoSpecials()
{
    MotorSpeedCommander commander;

    long timeLeft = 100;
    long clicksLeft = 200;

    long targetSpeed = commander.computeTargetSpeedTakingNegativeTimeIntoAccount( timeLeft, clicksLeft );

    TEST_ASSERT_EQUAL( (1000L * clicksLeft) / timeLeft, targetSpeed );

}

void testComputeTargetSpeedTakingNegativeTimeIntoAccountNegativeTime()
{
    MotorSpeedCommander commander;

    long timeLeft = -100;
    long clicksLeft = 200;
    commander.maxSpeedToMove = 300;

    long targetSpeed = commander.computeTargetSpeedTakingNegativeTimeIntoAccount( timeLeft, clicksLeft );

    TEST_ASSERT_EQUAL( 300, targetSpeed );
}

void testCapTargetSpeedNoCap()
{
    MotorSpeedCommander commander;
    commander.maxSpeedToMove = 300;
    long targetSpeed = 250;

    TEST_ASSERT_EQUAL( targetSpeed, commander.capTargetSpeed( targetSpeed) );
}

void testCapTargetSpeedCap()
{
    MotorSpeedCommander commander;
    commander.maxSpeedToMove = 300;
    long targetSpeed = 350;

    TEST_ASSERT_EQUAL( commander.maxSpeedToMove, commander.capTargetSpeed( targetSpeed) );
}

void testCapTargetSpeedLowCap()
{
    MotorSpeedCommander commander;
    commander.maxSpeedToMove = 300;
    long targetSpeed = -350;

    TEST_ASSERT_EQUAL( 1, commander.capTargetSpeed( targetSpeed) );
}

void testComputeTargetSpeed()
{
    MotorSpeedCommander commander;

    long timeLeft = 100;
    long clicksLeft = 200;
    commander.maxSpeedToMove = 300;

    long targetSpeed = commander.computeTargetSpeed( timeLeft, clicksLeft );

    TEST_ASSERT_EQUAL( commander.maxSpeedToMove, targetSpeed );
}

void testRunHasArrived()
{
    MotorSpeedCommander commander;
    MockHomingEncoder encoder;
    TestingMotorSpeedRegulator regulator;
    regulator.setPoint = 200;
    commander.config( &encoder, &regulator );
    commander.hasArrivedAtPos = false;
    commander.timeToArrive = 100;
    commander.posToMoveTo = RotationPositionWithLaps( 0, 1, 0 );
    encoder.pos = RotationPositionWithLaps( 10, 1, 0 );

    commander.run(0);

    TEST_ASSERT_TRUE( commander.hasArrived() );
    TEST_ASSERT_EQUAL( 200, regulator.setPoint ); //No change
}

void testRunHasNotArrived()
{
    MotorSpeedCommander commander;
    MockHomingEncoder encoder;
    TestingMotorSpeedRegulator regulator;
    regulator.setPoint = 200;
    commander.config( &encoder, &regulator );
    commander.hasArrivedAtPos = false;
    commander.timeToArrive = 100;
    commander.posToMoveTo = RotationPositionWithLaps( 10, 1, 0 );
    encoder.pos = RotationPositionWithLaps( 0, 1, 0 );

    commander.run(0);

    TEST_ASSERT_FALSE( commander.hasArrived() );
    TEST_ASSERT_EQUAL( 100, regulator.setPoint );
}

void testRunHasAllreadyArrived()
{
    MotorSpeedCommander commander;
    MockHomingEncoder encoder;
    TestingMotorSpeedRegulator regulator;
    regulator.setPoint = 200;
    commander.config( &encoder, &regulator );
    commander.hasArrivedAtPos = true;
    commander.timeToArrive = 100;
    commander.posToMoveTo = RotationPositionWithLaps( 10, 1, 0 );
    encoder.pos = RotationPositionWithLaps( 0, 1, 0 );

    commander.run(0);

    TEST_ASSERT_TRUE( commander.hasArrived() );
    TEST_ASSERT_EQUAL( 200, regulator.setPoint );
}

void processMotorSpeedCommander()
{
    RUN_TEST( testConfig );
    RUN_TEST( testInit );
    RUN_TEST( testComputeTargetSpeedTakingNegativeTimeIntoAccountNoSpecials );
    RUN_TEST( testComputeTargetSpeedTakingNegativeTimeIntoAccountNegativeTime );
    RUN_TEST( testCapTargetSpeedNoCap );
    RUN_TEST( testCapTargetSpeedCap );
    RUN_TEST( testCapTargetSpeedLowCap );
    RUN_TEST( testComputeTargetSpeed );
    RUN_TEST( testRunHasArrived );
    RUN_TEST( testRunHasNotArrived );
    RUN_TEST( testRunHasAllreadyArrived );
}