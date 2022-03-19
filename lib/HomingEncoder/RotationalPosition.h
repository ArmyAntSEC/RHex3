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

    RotationalPosition( long laps, int16_t clicks )
    {
        linPos = (laps * clicksPerLapNum) / clicksPerLapDen + clicks;
    }

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

    void moveToLapBeforeRounded( int16_t _clicks )
    {                        
        int16_t thisClicks = getClicks();
        long clicksDiff = thisClicks - _clicks;                
        if ( clicksDiff < 0 )
        {            
            linPos = (getLaps()+1)*10775776/3000 + thisClicks;
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
