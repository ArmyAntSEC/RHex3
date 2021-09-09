#ifndef _ROTATIONPOSITIONWITHLAP_H_
#define _ROTATIONPOSITIONWITHLAP_H_

#include <FixedPointsCommon.h>

class RotationPositionWithLaps
{
private:
    int clickPosition = 0;
    long int laps = 0;
    SQ1x14 remainder = 0;

    static SQ15x16 clicksPerRotation;
    

public:
    RotationPositionWithLaps(int _clickPosition = 0, long int _laps = 0, SQ1x14 _remainder = 0) : 
        clickPosition(_clickPosition), laps(_laps), remainder(_remainder)
    {    
        this->reduceToMinimalFormWithRecursion();
    }

    void moveForwardTo( int _clickPosition )
    {        
        if ( _clickPosition < this->clickPosition )
        {
            this->laps++;
        }
        this->clickPosition = _clickPosition;
        this->remainder = 0;
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

    int getClickPosition() const
    {
        return this->clickPosition;
    }

    int getLaps() const
    {
        return this->laps;
    }

    SQ1x14 getRemainder() const
    {
        return this->remainder;
    }

    long int getRemainderMicroClicks() const
    {
        return (long int)(((double)this->remainder)*1e6);
    }


    SQ15x16 getClicksPerRotation() const
    {
        return this->clicksPerRotation;
    }

    long getShortestPositiveDifferenceInt( RotationPositionWithLaps const * const pos2 )
    {
        long clicksDifference = this->getClickPosition() - pos2->getClickPosition();
        if ( clicksDifference < 0 ) {                    
            clicksDifference +=  this->getClicksPerRotation().getInteger();
        }

        return  clicksDifference;
    }

    long getDifferenceInClicks( RotationPositionWithLaps const * const pos2 ) const
    {        
        long clicksDifference = this->getClickPosition() - pos2->getClickPosition();
        long lapDifference = this->getLaps() - pos2->getLaps();        
        long clickDifferenceFromLaps = (lapDifference * this->getClicksPerRotation()).getInteger();
        //Log << "Lap difference: " << lapDifference << " clickDifference: " << clicksDifference << endl;
        return clicksDifference + clickDifferenceFromLaps;
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