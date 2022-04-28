#include <unityEx.h>
#include <SerialStream.h>
#include <MotorSpeedRegulatorInterfaces.h>

#define private public
#include <GaitCommander.h>

struct LegCommandParserMock: public MotorSpeedCommanderInterface
{
    LegCommand lastCommand;
    int32_t timeNow;
    bool commandReceived = false;

    virtual void setGoal(LegCommand command, int32_t _timeNow ) 
    {
        lastCommand = command;
        timeNow = _timeNow;
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

    int32_t stepTimeStart = 2e6;
    int16_t slowStartPos = 100;    
    int16_t slowTimePercent = 75;
    int16_t slowLength = 1000;
    int32_t period = 1e6L;
    sut.config( stepTimeStart, slowStartPos, slowTimePercent, slowLength, period );

    TEST_ASSERT_EQUAL( 100, sut.slowStartPos );    
    TEST_ASSERT_EQUAL( 1100, sut.fastStartPos );
    TEST_ASSERT_EQUAL( 0.75e6, sut.fastRelativeStartTimeMicros );
    TEST_ASSERT_EQUAL( period, sut.stepTimeTotalMicros );
    TEST_ASSERT_EQUAL( &parser, sut.parser );
}

void testRunLegCommandSequence()
{        
    LegCommandParserMock parser;
    LegCommandSequence sut( &parser );

    int32_t stepTimeStart = 2e6;
    int16_t slowStartPos = 100;    
    int16_t slowTimePercent = 75;
    int16_t slowLength = 1000;
    int32_t stepTimeTotal = 1e6L;
    
    sut.config( stepTimeStart, slowStartPos, slowTimePercent, slowLength, stepTimeTotal );

    sut.run( 2e6 );
    TEST_ASSERT_TRUE( parser.commandReceived );    
    TEST_ASSERT_EQUAL( 1100, parser.lastCommand.targetPositionClicks );
    TEST_ASSERT_EQUAL( 2.75e6L, parser.lastCommand.targetTimeMicros );
    
    parser.reset(); 
    sut.run( 2.75e6 - 1 ); //Should not issue a new command as there is no change.
    TEST_ASSERT_FALSE( parser.commandReceived );    

    parser.reset(); 
    sut.run( 2.75e6 ); //Should issue a new command.
    TEST_ASSERT_TRUE( parser.commandReceived );    
    TEST_ASSERT_EQUAL( 100, parser.lastCommand.targetPositionClicks );
    TEST_ASSERT_EQUAL( 3e6, parser.lastCommand.targetTimeMicros );    

    parser.reset(); 
    sut.run( 3e6+1 ); //Should not issue a new command as we only do one period.
    TEST_ASSERT_FALSE( parser.commandReceived );        
}


void testGaitCommanderWithOneLeg()
{    
    LegCommandParserMock parser;
    LegCommandSequence leg( &parser );
    int32_t stepTimeStart = 0;
    int16_t slowStartPos = 100;    
    int16_t slowTimePercent = 50;
    int16_t slowLength = 1000;
    int32_t stepTimeTotal = 2e6;
    leg.config( stepTimeStart, slowStartPos, slowTimePercent, slowLength, stepTimeTotal );

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
    RUN_TEST( testGaitCommanderWithOneLeg );
    UNITY_END_INT();

}