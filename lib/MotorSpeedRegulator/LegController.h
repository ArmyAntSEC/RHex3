#ifndef _LEGCONTROLLER_H_
#define _LEGCONTROLLER_H_

#include <RecurringTaskBase.h>

class LegController: public RecurringTaskBase
{
    private:
        const unsigned int clicksPerRev = 3592;
        int posSequence[2][2];        
        //unsigned long int nextRun = 0;

        
        MotorSpeedCommander* commander = 0;
        enum StepSequence { Init, Fast, Slow, Stop };
        StepSequence stepSequence = Stop;
    public:
        enum EndPos { AfterStep, BeforeStep };
        
        virtual bool canRun( unsigned long int now ) 
        {        
            return RecurringTaskBase::canRun(now);
        }
    
        virtual void init( unsigned long int now, int stepTime, int slowTime, int slowAngle, EndPos endPos )
        {
            int slowHalfAngle = slowAngle / 2;            
            posSequence[0][0] = stepTime - slowTime; //Time to finish fast
            posSequence[0][1] = clicksPerRev - slowHalfAngle; //Pos to finish on fast
            posSequence[1][0] = stepTime; //Time to finish slow
            posSequence[1][1] = slowHalfAngle;   //Pos to end on slow
            stepSequence = Init;     
        }
        
        virtual void config(  MotorSpeedCommander* _commander )
        {
            this->commander = _commander;
        }

        virtual void run( unsigned long int _now )
        {
            if( commander == 0 ) {
                return;
            }

            switch ( stepSequence ) {
                case Init:                    
                    Log << "Init happened. Step started. Goal: " << posSequence[0][1] << endl;
                    commander->init( _now, posSequence[0][0], posSequence[0][1] ); //Never move more than one lap, for now                       
                    stepSequence = Fast;
                    break;                
                case Fast:
                    if ( commander->hasArrived() ) {                        
                        commander->init( _now, posSequence[1][0], posSequence[1][1] ); //Never move more than one lap, for now
                        stepSequence = Slow;                        
                        Log << "Arrived at after fast phase. Goal: " << posSequence[1][1] << endl;                        
                    }
                default:
                    break;
            }
        }

        int* getPosSequence( int i ) { return this->posSequence[i]; } //Only for testing         
};

#endif