#ifndef _LEGCONTROLLER_H_
#define _LEGCONTROLLER_H_

#include <RecurringTaskBase.h>

class LegController: public RecurringTaskBase
{
    private:
        const unsigned int clicksPerRev = 3592;
        unsigned long int posSequence[2][2];        
        unsigned long int nextRun = 0;

        MotorSpeedCommander* commander;

    public:
        enum EndPos { AfterStep, BeforeStep };
        
        virtual bool canRun( unsigned long int now ) 
        {        
            return RecurringTaskBase::canRun(now) && now > nextRun;
        }
    
        virtual void init( unsigned long int now, int stepTime, int slowTime, int slowAngle, EndPos endPos )
        {
            int slowHalfAngle = slowAngle / 2;            
            posSequence[0][0] = now + stepTime - slowTime;
            posSequence[0][1] = clicksPerRev - slowHalfAngle; 
            posSequence[1][0] = now + stepTime;
            posSequence[1][1] = slowHalfAngle;
        }
        
        virtual void config(  MotorSpeedCommander* _commander )
        {
            this->commander = _commander;
        }

        virtual void run( unsigned long int _now )
        {

        }

        unsigned long int* getPosSequence( int i ) { return this->posSequence[i]; } //Only for testing         
};

#endif