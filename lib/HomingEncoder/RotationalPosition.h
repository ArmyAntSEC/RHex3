#pragma once

#include "HomingEncoderInterfaces.h"
#include <SerialStream.h>

class RotationalPositionProvider
{
protected:
    //673486 / 375 = 1795,962667 = Clicks per channel for a rotation.
    static const int32_t clicksPerLapNum = 673486; 
    static const int32_t clicksPerLapDen = 375;
    

public:
    virtual int32_t getLinearPosition() const = 0;          

    virtual int32_t getLaps() const
    {
        int64_t clickPosRaw = getLinearPosition();
        return clickPosRaw * clicksPerLapDen / clicksPerLapNum;
    }

    virtual int32_t getClicks() const
    {
        int64_t laps = getLaps();
        int64_t clickPosRaw = getLinearPosition();
        int32_t clicksRemain = clickPosRaw - laps * clicksPerLapNum / clicksPerLapDen;
        return clicksRemain;
    }      
};

class RotationalPosition: public RotationalPositionProvider
{
private:
    int32_t linPos;

public:
    RotationalPosition( int32_t _linPos = 0 ): linPos(_linPos)
    {}

    RotationalPosition( int32_t laps, int16_t clicks )
    {
        linPos = (laps * clicksPerLapNum) / clicksPerLapDen + clicks;
    }

    virtual int32_t getLinearPosition() const {
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
        int32_t clicksDiff = thisClicks - _clicks;                
        if ( clicksDiff < 0 )
        {            
            linPos = (getLaps()+1)*clicksPerLapNum/clicksPerLapDen + thisClicks;
        }       
    }
};

class RotationalPositionEncoder: public RotationalPositionProvider
{
private:
    LinearPositionProvider* linPos;

    virtual int32_t getLinearPosition() const {
        return linPos->getLinearPosition();
    }

public:
    RotationalPositionEncoder( LinearPositionProvider* _linPos ): linPos(_linPos)
    {}
};
