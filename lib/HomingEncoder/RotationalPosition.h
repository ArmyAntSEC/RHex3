#pragma once

#include "HomingEncoderInterfaces.h"

class RotationalPosition: public RotationalPositionProvider
{
private:
    long linPos;        
public:
    RotationalPosition( long _linPos = 0 ): linPos(_linPos)
    {}

    virtual long getLaps()
    {
        long long clickPosRaw = linPos;
        return clickPosRaw * clicksPerLapDen / clicksPerLapNum;
    }

    virtual long getClicks()
    {
        long long laps = getLaps();
        long long clickPosRaw = linPos;
        long clicksRemain = clickPosRaw - laps * clicksPerLapNum / clicksPerLapDen;
        return clicksRemain;
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

