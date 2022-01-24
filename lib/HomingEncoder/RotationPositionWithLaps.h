#ifndef _ROTATIONPOSITIONWITHLAP_H_
#define _ROTATIONPOSITIONWITHLAP_H_

class RotationPositionWithLaps
{
private:
    int clickPosition = 0;
    long int laps = 0;
    int remainder = 0;

    static int clicksPerRotation;
    

public:
    RotationPositionWithLaps(int _clickPosition = 0, long int _laps = 0, int _remainder = 0) : 
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
        if ( this->clickPosition > this->clicksPerRotation )
        {                
            this->removeOneLap();        
            this->reduceToMinimalFormWithRecursion();
        }
    }

    void removeOneLap()
    {
        long precise_position = ((long int)this->clickPosition) + (this->remainder) - this->clicksPerRotation;
        this->clickPosition = precise_position;
        this->remainder = precise_position-precise_position;
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

    int getRemainder() const
    {
        return this->remainder;
    }

    long int getRemainderMicroClicks() const
    {
        return (long int)(((double)this->remainder)*1e6);
    }


    int getClicksPerRotation() const
    {
        return this->clicksPerRotation;
    }

    long getShortestPositiveDifferenceInt( RotationPositionWithLaps const * const pos2 )
    {
        long clicksDifference = this->getClickPosition() - pos2->getClickPosition();
        if ( clicksDifference < 0 ) {                    
            clicksDifference +=  this->getClicksPerRotation();
        }

        return  clicksDifference;
    }

    long getDifferenceInClicks( RotationPositionWithLaps const * const pos2 ) const
    {        
        long clicksDifference = this->getClickPosition() - pos2->getClickPosition();
        long lapDifference = this->getLaps() - pos2->getLaps();        
        long clickDifferenceFromLaps = (lapDifference * this->getClicksPerRotation());
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
        int clicksFromClicks = this->getClickPosition();
        int clicksFromLaps = int(this->getClicksPerRotation() * this->getLaps());
        int clicksFromRemainder = int( this->getRemainder() );


        return clicksFromClicks + clicksFromLaps + clicksFromRemainder;
    }
};

#endif