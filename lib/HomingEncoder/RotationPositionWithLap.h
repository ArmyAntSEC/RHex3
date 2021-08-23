#ifndef _ROTATIONPOSITIONWITHLAP_H_
#define _ROTATIONPOSITIONWITHLAP_H_

#include <FixedPointsCommon.h>

class RotationPositionWithLap
{
private:
    int clickPosition;
    long int laps;
    SQ15x16 clicksPerRotation = 1795.9626665;
    SQ1x14 remainder = 0;

public:
    RotationPositionWithLap(int _clickPosition, long int _laps) : clickPosition(_clickPosition), laps(_laps)
    {    
        this->reduceToMinimalFormWithRecursion();
    }

    void incrementClicks( int clicks ) 
    {
        this->clickPosition += clicks;
        this->reduceToMinimalFormWithRecursion();
    }
    
    void reduceToMinimalFormWithRecursion()
    {
        if ( this->clickPosition > this->clicksPerRotation.getInteger() )
        {                
            this->removeOneLap();        
            this->reduceToMinimalFormWithRecursion();
        }
    }

    void removeOneLap()
    {
        SQ15x16 precise_position = SQ15x16((long int)this->clickPosition) + SQ15x16(this->remainder) - this->clicksPerRotation;
        this->clickPosition = floorFixed(precise_position).getInteger();
        this->remainder = SQ1x14(precise_position-floorFixed(precise_position));
        this->laps++;            
    }

    int getClickPosition()
    {
        return this->clickPosition;
    }

    int getLaps()
    {
        return this->laps;
    }

    SQ1x14 getRemainder()
    {
        return this->remainder;
    }

    SQ15x16 getClicksPerRotation()
    {
        return this->clicksPerRotation;
    }
};

#endif