#pragma once

#include "HomingEncoderInterfaces.h"
#include <SerialStream.h>

class RotationalPositionProvider
{
protected:
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

    bool operator==( const RotationalPositionProvider& pos )
    {
        return linPos == pos.getLinearPosition();
    }

    bool operator!=( const RotationalPositionProvider& pos )
    {
        return !this->operator==( pos );        
    }

    void moveToLapBeforeRounded( int _clicks )
    {                        
        long clicksDiff = getClicks() - _clicks;                
        if ( clicksDiff < 0 )
        {            
            linPos += clicksPerLapNum/clicksPerLapDen;
        }        
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
