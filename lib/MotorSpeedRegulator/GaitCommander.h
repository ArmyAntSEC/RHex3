#pragma once

#include <MotorSpeedRegulatorInterfaces.h>
#include <RunnableInterface.h>

struct LegCommandSequence: public RunnableInterface
{    
    int slowStartPos;    
    int fastStartPos;
    long fastStartTimeMicros;    
    long periodMicros;
    bool firstCommandSent = false;
    bool inFastSegment = true;
    LegCommandControllerInterface* parser;

    LegCommandSequence( LegCommandControllerInterface* _parser ): parser(_parser)
    {}

    void config( int _slowStartPos, int _slowTimePercent, int _slowLength, long _period )
    {
        slowStartPos = _slowStartPos;        
        fastStartPos = _slowStartPos + _slowLength;
        fastStartTimeMicros = _slowTimePercent * _period / 100L;
        periodMicros = _period;        
    }

    void run ( unsigned long _nowMicros )
    {
        //Find out where we are in our loop
        int timeInLoop = _nowMicros % periodMicros;
        long loopNumber = _nowMicros / periodMicros;

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


template <int MaxLegs>
class GaitCommander: public RunnableInterface
{
private:        
    LegCommandSequence* legSequenceList[MaxLegs];    
    int legCount = 0;

public:        
    void addLegSchedule( LegCommandSequence* _sequence )
    {
        legSequenceList[legCount++] = _sequence;
    }

    void run( unsigned long _nowMicros )
    {
        for ( int i = 0; i < legCount; i++ )
        {
            legSequenceList[i]->run( _nowMicros );
        }
    }
};

