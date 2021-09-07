#ifndef _VOLATILEROTATIONPOSITIONWITHLAPS_H_
#define _VOLATILEROTATIONPOSITIONWITHLAPS_H_

#include <RotationPositionWithLaps.h>

class VolatileRotationPositionWithLaps
{
private:
    volatile long int clickPosition = 0;
    volatile long int laps = 0;
    volatile long int remainderInMicroClick = 0;

    static const long int clicksPerRotationIntPart;
    static const long int clicksPerRotationMicroClickFractionPart;

public:
    VolatileRotationPositionWithLaps(int _clickPosition=0, long int _laps=0, long int _remainder = 0) : 
        clickPosition(_clickPosition), laps(_laps), remainderInMicroClick(_remainder)
    {
        reduceToMinimalForm();
    }

    void reduceToMinimalForm()
    {
        if (clickPosition > clicksPerRotationIntPart)
        {
            removeOneLap();
            reduceToMinimalForm();
        }
    }

    void removeOneLap()
    {
        clickPosition -= clicksPerRotationIntPart;
        laps++;
        remainderInMicroClick -= clicksPerRotationMicroClickFractionPart;
        if (remainderInMicroClick < 0)
        {
            remainderInMicroClick += 1e6;
            clickPosition--;
        }
    }

    long int getClickPosition()
    {
        return clickPosition;
    }

    long int getLaps()
    {
        return laps;
    }

    long int getRemainderMicroClicks()
    {
        return remainderInMicroClick;
    }

    void incrementMe()
    {
        clickPosition++;        
    }
    
    RotationPositionWithLaps getNonVolatileCopy()
    {
        return RotationPositionWithLaps( clickPosition, laps, 0 );
    }
};

#endif