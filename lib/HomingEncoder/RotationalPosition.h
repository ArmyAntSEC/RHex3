#pragma once

#include "HomingEncoderInterfaces.h"

class RotationalPositionProvider
{
private:
    static const long clicksPerLapNum = 10775776;
    static const long clicksPerLapDen = 3000;

public:
    virtual long getLinearPosition() const = 0;          

    virtual long getLaps() const
    {
        long long clickPosRaw = getLinearPosition();
        return clickPosRaw * clicksPerLapDen / clicksPerLapNum;
    }

    virtual long getClicks() const
    {
        long long laps = getLaps();
        long long clickPosRaw = getLinearPosition();
        long clicksRemain = clickPosRaw - laps * clicksPerLapNum / clicksPerLapDen;
        return clicksRemain;
    }      

    int angularDifference( const RotationalPositionProvider& pos )
    {
        long difference = this->getClicks() - pos.getClicks();
        if ( difference < 0 ) { //TODO: This should be possible with a modulo operator as well.
            difference = difference + clicksPerLapNum/clicksPerLapDen;
        }
        return difference;
    }
};

class RotationalPosition: public RotationalPositionProvider
{
private:
    long linPos;

public:
    RotationalPosition( long _linPos = 0 ): linPos(_linPos)
    {}

    virtual long getLinearPosition() const {
        return linPos;
    }

    bool operator==( const RotationalPosition& pos )
    {
        return linPos == pos.linPos;
    }

    bool operator!=( const RotationalPosition& pos )
    {
        return !this->operator==( pos );        
    }
};

class RotationalPositionEncoder: public RotationalPositionProvider
{
private:
    LinearPositionProvider* linPos;

    virtual long getLinearPosition() const {
        return linPos->getLinearPosition();
    }

public:
    RotationalPositionEncoder( LinearPositionProvider* _linPos ): linPos(_linPos)
    {}
};
