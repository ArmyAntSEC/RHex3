#pragma once

#include <MotorSpeedRegulatorInterfaces.h>
#include <RunnableInterface.h>

struct LegCommandSequence: public RunnableInterface
{    
    int16_t slowStartPos;    
    int16_t fastStartPos;
    int32_t fastStartTimeMicros;    
    int32_t periodMicros;
    bool firstCommandSent = false;
    bool inFastSegment = true;
    LegCommandControllerInterface* parser;

    LegCommandSequence( LegCommandControllerInterface* _parser ): parser(_parser)
    {}

    void config( int16_t _slowStartPos, int16_t _slowTimePercent, int16_t _slowLength, int32_t _period )
    {
        slowStartPos = _slowStartPos;        
        fastStartPos = _slowStartPos + _slowLength;
        fastStartTimeMicros = _slowTimePercent * _period / 100L;
        periodMicros = _period;        
    }

    void run ( int32_t _nowMicros )
    {
        //Find out where we are in our loop
        int32_t timeInLoop = _nowMicros % periodMicros;
        int32_t loopNumber = _nowMicros / periodMicros;

        LegCommandControllerInterface::LegCommand command;                
        
        bool inFastSegmentNow = timeInLoop >= fastStartTimeMicros;        

        if ( inFastSegmentNow != inFastSegment || !firstCommandSent ) {
            firstCommandSent = true;
            inFastSegment = inFastSegmentNow;

            if ( inFastSegmentNow ) {
                command.targetPositionClicks = slowStartPos;
                command.targetTimeMicros = periodMicros*(loopNumber+1);
            } else {
                command.targetPositionClicks = fastStartPos;
                command.targetTimeMicros = fastStartTimeMicros + periodMicros*loopNumber;            
            }        

            parser->receiveLegCommand( command );
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

