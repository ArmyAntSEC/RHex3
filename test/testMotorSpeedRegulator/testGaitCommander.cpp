#include <unityEx.h>
#include <SerialStream.h>
#include <MotorSpeedRegulatorInterfaces.h>

#define private public
#include <GaitCommander.h>

struct LegCommandParserMock: public LegCommandParserInterface
{
    LegCommand lastCommand;

    virtual void receiveLegCommand(LegCommand command) 
    {
        lastCommand = command;
    }
};

void testCreateLegCommandSequence()
{
    LegCommandSequenceCompact legSequence;
    legSequence.slowStartPos = 0;    
    legSequence.slowTimePercent = 50;
    legSequence.slowLength = 1000;
    long period = 2e6;

    LegCommandParserMock parser;

    LegCommandSequence sut( &legSequence, &parser, period );

    TEST_ASSERT_EQUAL( 0, sut.slowStartPos );
    TEST_ASSERT_EQUAL( 0, sut.slowStartTimeMicros );
    TEST_ASSERT_EQUAL( 1000, sut.fastStartPos );
    TEST_ASSERT_EQUAL( 1e6, sut.fastStartTimeMicros );
    TEST_ASSERT_EQUAL( period, sut.period );
    TEST_ASSERT_EQUAL( &parser, sut.parser );
}

void testRunLegCommandSequence()
{    
    LegCommandSequenceCompact legSequence;
    legSequence.slowStartPos = 0;    
    legSequence.slowTimePercent = 50;
    legSequence.slowLength = 1000;
    long period = 2e6;

    LegCommandParserMock parser;

    LegCommandSequence sut( &legSequence, &parser, period );

    sut.run( 0 );
    TEST_ASSERT_EQUAL( 1000, parser.lastCommand.targetPositionClicks );
    TEST_ASSERT_EQUAL( 1e6, parser.lastCommand.targetTimeMicros );


}

void testShallControllOneLegForOneRound()
{
    TEST_IGNORE();
    LegCommandParserMock legCommandParser;
    GaitCommander<2> sut(&legCommandParser);
    sut.setPeriod( 2*1e6L );
    LegCommandSequenceCompact legSequence;
    legSequence.slowStartPos = 0;    
    legSequence.slowTimePercent = 50;
    legSequence.slowLength = 1000;

    sut.setLegSchedule<0>( legSequence );

    sut.run( 0 );    
    TEST_ASSERT_EQUAL( 1000, legCommandParser.lastCommand.targetPositionClicks );
    TEST_ASSERT_EQUAL( 1e6, legCommandParser.lastCommand.targetTimeMicros );

    legCommandParser.lastCommand = LegCommandParserInterface::LegCommand();

    sut.run( 999 );    

    TEST_ASSERT_EQUAL( 0, legCommandParser.lastCommand.targetPositionClicks );
    TEST_ASSERT_EQUAL( 0, legCommandParser.lastCommand.targetTimeMicros );


}

void runAllTestGaitCommander()
{    
    UNITY_BEGIN_INT();    
    RUN_TEST( testCreateLegCommandSequence );
    RUN_TEST( testRunLegCommandSequence );
    RUN_TEST( testShallControllOneLegForOneRound );
    UNITY_END_INT();

}