#pragma once

#include "HomingEncoderInterfaces.h"

class RotationalPositionProvider
{
private:
    static const long clicksPerLapNum = 10775776;
    static const long clicksPerLapDen = 3000;

protected:
    virtual long getLinearPosition() = 0;          

public:
    virtual long getLaps()
    {
        long long clickPosRaw = getLinearPosition();
        return clickPosRaw * clicksPerLapDen / clicksPerLapNum;
    }

    virtual long getClicks()
    {
        long long laps = getLaps();
        long long clickPosRaw = getLinearPosition();
        long clicksRemain = clickPosRaw - laps * clicksPerLapNum / clicksPerLapDen;
        return clicksRemain;
    }      
};

class RotationalPosition: public RotationalPositionProvider
{
private:
    long linPos;

public:
    RotationalPosition( long _linPos = 0 ): linPos(_linPos)
    {}

    virtual long getLinearPosition() {
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

    virtual long getLinearPosition() {
        return linPos->getLinearPosition();
    }

public:
    RotationalPositionEncoder( LinearPositionProvider* _linPos ): linPos(_linPos)
    {}

};
