#pragma once

#include <MotorSpeedRegulatorInterfaces.h>
#include <RunnableInterface.h>

struct LegCommandSequenceCompact
{    
    int slowStartPos;
    int slowTimePercent;
    int slowLength;
};

struct LegCommandSequence
{    
    int slowStartPos;
    int slowStartTimeMicros;
    int fastStartPos;
    int fastStartTimeMicros;

    LegCommandSequence( LegCommandSequenceCompact& sequence, int period )
    {
        slowStartPos = sequence.slowStartPos;
        slowStartTimeMicros = 0;
        fastStartPos = sequence.slowStartPos + sequence.slowLength;
        fastStartTimeMicros = sequence.slowTimePercent * period / 100;
    }
};


template <int MaxLegs>
class GaitCommander: public RunnableInterface
{
private:    
    long period; 
    LegCommandSequenceCompact legSequenceList[MaxLegs];

    LegCommandParserInterface* legCommander;

public:
    GaitCommander( LegCommandParserInterface* _legCommander ): legCommander(_legCommander)
    {
    }

    void setPeriod( long _period )
    {
        period = _period;
    }

    template <int LegNum>
    void setLegSchedule( LegCommandSequenceCompact _sequence)
    {
        legSequenceList[LegNum] = _sequence;
    }

    void run( unsigned long _now )
    {
        LegCommandParserInterface::LegCommand command;
        command.targetPositionClicks = legSequenceList[0].slowLength;
        command.targetTimeMicros = legSequenceList[0].slowTimePercent * period / 100;
        legCommander->receiveLegCommand( command );
    }
};

