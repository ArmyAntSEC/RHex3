#include <unityEx.h>
#include <SerialStream.h>
#include <MotorSpeedRegulatorInterfaces.h>

#define private public
#include <GaitCommander.h>

struct LegCommandParserMock: public LegCommandControllerInterface
{
    LegCommand lastCommand;
    bool commandReceived = false;

    virtual void receiveLegCommand(LegCommand command) 
    {
        lastCommand = command;
        commandReceived = true;
    }

    void reset()
    {
        lastCommand = LegCommand();
        commandReceived = false;
    }
};

void testCreateLegCommandSequence()
{    
    LegCommandParserMock parser;
    LegCommandSequence sut( &parser );

    int slowStartPos = 100;    
    int slowTimePercent = 50;
    int slowLength = 1000;
    long period = 2e6L;
    sut.config( slowStartPos, slowTimePercent, slowLength, period );

    TEST_ASSERT_EQUAL( 100, sut.slowStartPos );    
    TEST_ASSERT_EQUAL( 1100, sut.fastStartPos );
    TEST_ASSERT_EQUAL( 1e6, sut.fastStartTimeMicros );
    TEST_ASSERT_EQUAL( period, sut.periodMicros );
    TEST_ASSERT_EQUAL( &parser, sut.parser );
}

void testRunLegCommandSequence()
{    
    LegCommandParserMock parser;
    LegCommandSequence sut( &parser );

    int slowStartPos = 100;    
    int slowTimePercent = 50;
    int slowLength = 1000;
    long period = 2e6L;
    sut.config( slowStartPos, slowTimePercent, slowLength, period );

    sut.run( 0 );
    TEST_ASSERT_TRUE( parser.commandReceived );    
    TEST_ASSERT_EQUAL( 1100, parser.lastCommand.targetPositionClicks );
    TEST_ASSERT_EQUAL( 1e6L, parser.lastCommand.targetTimeMicros );
    
    parser.reset(); 
    sut.run( 1e6 - 1 ); //Should not issue a new command as there is no change.
    TEST_ASSERT_FALSE( parser.commandReceived );    

    sut.run( 1e6 ); //Should issue a new command.
    TEST_ASSERT_TRUE( parser.commandReceived );    
    TEST_ASSERT_EQUAL( 100, parser.lastCommand.targetPositionClicks );
    TEST_ASSERT_EQUAL( 2e6, parser.lastCommand.targetTimeMicros );    
}

void testRunLegCommandSequenceThirdStep()
{    
    LegCommandParserMock parser;
    LegCommandSequence sut( &parser );

    int slowStartPos = 100;    
    int slowTimePercent = 50;
    int slowLength = 1000;
    long period = 2e6;
    sut.config( slowStartPos, slowTimePercent, slowLength, period );

    sut.run( 4e6 - 1 );
    TEST_ASSERT_EQUAL( 100, parser.lastCommand.targetPositionClicks );
    TEST_ASSERT_EQUAL( 4e6, parser.lastCommand.targetTimeMicros );

    sut.run( 4e6 );
    TEST_ASSERT_EQUAL( 1100, parser.lastCommand.targetPositionClicks );
    TEST_ASSERT_EQUAL( 5e6, parser.lastCommand.targetTimeMicros );
}

void testGaitCommanderWithOneLeg()
{    
    LegCommandParserMock parser;
    LegCommandSequence leg( &parser );
    int slowStartPos = 100;    
    int slowTimePercent = 50;
    int slowLength = 1000;
    long period = 2e6;
    leg.config( slowStartPos, slowTimePercent, slowLength, period );

    GaitCommander<2> sut;

    sut.addLegSchedule(&leg);
    sut.run( 4e6 );
    
    TEST_ASSERT_TRUE( parser.commandReceived );        
}

void runAllTestGaitCommander()
{    
    UNITY_BEGIN_INT();    
    RUN_TEST( testCreateLegCommandSequence );
    RUN_TEST( testRunLegCommandSequence );
    RUN_TEST( testRunLegCommandSequenceThirdStep );
    RUN_TEST( testGaitCommanderWithOneLeg );
    UNITY_END_INT();

}