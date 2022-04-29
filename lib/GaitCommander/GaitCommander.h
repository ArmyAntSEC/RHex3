#pragma once

#include <MotorSpeedRegulatorInterfaces.h>
#include <RunnableInterface.h>

struct LegCommandSequence: public RunnableInterface
{    
    int32_t stepTimeStartMicros;
    int16_t slowStartPos;    
    int16_t fastStartPos;
    int32_t fastRelativeStartTimeMicros;    
    int32_t stepTimeTotalMicros;
    bool firstCommandSent = false;
    bool inFastSegment = true;
    MotorSpeedCommanderInterface* parser;

    LegCommandSequence( MotorSpeedCommanderInterface* _parser ): parser(_parser)
    {}

    void config( int32_t _stepTimeStart, int16_t _slowStartPos, int16_t _slowTimePercent, int16_t _slowLength, int32_t _stepTimeTotal )
    {
        stepTimeStartMicros = _stepTimeStart;
        slowStartPos = _slowStartPos;        
        fastStartPos = _slowStartPos + _slowLength;
        fastRelativeStartTimeMicros =  _slowTimePercent * _stepTimeTotal / 100L;
        stepTimeTotalMicros = _stepTimeTotal;                
    }

    void run ( int32_t _nowMicros )
    {                
        int32_t timeInLoop = _nowMicros - stepTimeStartMicros;        
        
        MotorCommanderGoal command;                
        
        bool inFastSegmentNow = timeInLoop >= fastRelativeStartTimeMicros;        

        if ( inFastSegmentNow != inFastSegment || !firstCommandSent ) {
            firstCommandSent = true;
            inFastSegment = inFastSegmentNow;

            if ( inFastSegmentNow ) {
                command = MotorCommanderGoal( slowStartPos,  stepTimeStartMicros + stepTimeTotalMicros );
            } else {
                command = MotorCommanderGoal( fastStartPos, stepTimeStartMicros + fastRelativeStartTimeMicros );
            }        

            parser->setGoal( command, _nowMicros );            
        }
    }
};


template <int16_t MaxLegs>
class GaitCommander: public RunnableInterface
{
private:        
    LegCommandSequence* legSequenceList[MaxLegs];    
    int16_t legCount = 0;

public:        
    void addLegSchedule( LegCommandSequence* _sequence )
    {
        legSequenceList[legCount++] = _sequence;
    }

    void run( int32_t _nowMicros )
    {
        for ( int16_t i = 0; i < legCount; i++ )
        {
            legSequenceList[i]->run( _nowMicros );
        }
    }
};

