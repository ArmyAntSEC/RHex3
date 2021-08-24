#ifndef _ROTATIONPOSITIONWITHLAP_H_
#define _ROTATIONPOSITIONWITHLAP_H_

#include <FixedPointsCommon.h>

class RotationPositionWithLaps
{
private:
    int clickPosition;
    long int laps;
    SQ1x14 remainder;

    static SQ15x16 clicksPerRotation;
    

public:
    RotationPositionWithLaps(int _clickPosition, long int _laps) : clickPosition(_clickPosition), laps(_laps)
    {    
        this->reduceToMinimalFormWithRecursion();
    }

    RotationPositionWithLaps(int _clickPosition, long int _laps, SQ1x14 _remainder) : 
        clickPosition(_clickPosition), laps(_laps), remainder(_remainder)
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

    int getShortestPositiveDifferenceInt( RotationPositionWithLaps* pos2 )
    {
        long clicksDifference = this->getClickPosition() - pos2->getClickPosition();
        if ( clicksDifference < 0 ) {                    
            clicksDifference +=  this->getClicksPerRotation().getInteger();
        }

        return  clicksDifference;
    }

    bool operator ==(const RotationPositionWithLaps &_pos2) const
    {
        RotationPositionWithLaps pos1(*this);
        RotationPositionWithLaps pos2(_pos2);

        pos1.reduceToMinimalFormWithRecursion();
        pos2.reduceToMinimalFormWithRecursion();

        bool clicksEqual = pos1.getClickPosition() == pos2.getClickPosition();
        bool lapsEqual = pos1.getLaps() == pos2.getLaps();
        bool remaindersEqual = pos1.getRemainder() == pos2.getRemainder();

        return clicksEqual && lapsEqual && remaindersEqual;
    }

    long int getSerialPosition()
    {
        SQ31x32 clicksFromClicks = this->getClickPosition();
        SQ31x32 clicksFromLaps = SQ31x32(this->getClicksPerRotation() * this->getLaps());
        SQ31x32 clicksFromRemainder = SQ31x32( this->getRemainder() );


        return (clicksFromClicks + clicksFromLaps + clicksFromRemainder).getInteger();
    }
};

#endif