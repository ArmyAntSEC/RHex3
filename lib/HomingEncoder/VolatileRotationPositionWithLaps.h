#ifndef _VOLATILEROTATIONPOSITIONWITHLAPS_H_
#define _VOLATILEROTATIONPOSITIONWITHLAPS_H_

#include <RotationPositionWithLaps.h>
#include <SerialStream.h>
#include <HardwareInterface.h>

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
        HardwareInterface::disableInterrupts();
        reduceToMinimalFormISR();
        HardwareInterface::enableInterrupts();
    }

    void reduceToMinimalFormISR()
    {        
        if (clickPosition > clicksPerRotationIntPart)
        {
            removeOneLapISR();
            reduceToMinimalForm();
        }        
    }

    void removeOneLapISR()
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

    long int getClickPositionISR()
    {        
        return clickPosition;     
    }

    long int getLapsISR()
    {        
        return laps;     
    }

    long int getRemainderMicroClicksISR()
    {        
        return remainderInMicroClick;     
    }

    void incrementMe()
    {
        HardwareInterface::disableInterrupts();        
        clickPosition++;        
        HardwareInterface::enableInterrupts();
    }
    
    RotationPositionWithLaps getNonVolatileCopy()
    {
        HardwareInterface::disableInterrupts();
        double fractionalRemainder = ((double)remainderInMicroClick)/1e6;        
        RotationPositionWithLaps rValue = RotationPositionWithLaps( clickPosition, laps, fractionalRemainder );
        HardwareInterface::enableInterrupts();
        return rValue;
    }
};

#endif