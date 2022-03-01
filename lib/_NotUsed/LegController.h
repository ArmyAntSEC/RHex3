#ifndef _LEGCONTROLLER_H_
#define _LEGCONTROLLER_H_

#include <RecurringTaskBase.h>

class LegController: public RecurringTaskBase
{
    private:
        const unsigned int clicksPerRev = 3592;
        int posSequence[2][2];                
        
        MotorSpeedCommander* commander = 0;
        enum StepSequence { Init, First, Second, Stop };
        StepSequence stepSequence = Stop;
    public:
        enum EndPos { AfterStep, BeforeStep };
        
        virtual bool canRun( unsigned long int now ) 
        {        
            return RecurringTaskBase::canRun(now);
        }
    
        virtual void init( unsigned long int now, int stepTime, int slowTime, int slowAngle, EndPos endPos )
        {
            RecurringTaskBase::init();
            int slowHalfAngle = slowAngle / 2;            
            switch ( endPos ) {
                case LegController::AfterStep:
                    posSequence[0][0] = now + stepTime - slowTime; //Time to finish fast
                    posSequence[0][1] = clicksPerRev - slowHalfAngle; //Pos to finish on fast
                    posSequence[1][0] = now + stepTime; //Time to finish slow
                    posSequence[1][1] = slowHalfAngle;   //Pos to end on slow
                    break;
                case LegController::BeforeStep:                
                    posSequence[0][0] = now + slowTime; //Time to finish slow
                    posSequence[0][1] = slowHalfAngle;   //Pos to end on slow
                    posSequence[1][0] = now + stepTime; //Time to finish fast
                    posSequence[1][1] = clicksPerRev - slowHalfAngle; //Pos to finish on fast                    
                    break;
            }
            stepSequence = Init;     
        }
        
        virtual void config(  MotorSpeedCommander* _commander )
        {
            this->commander = _commander;
        }

        virtual void run( unsigned long int _now )
        {
            //Log << "Ping" << endl;
            if( commander == 0 ) {
                return;
            }

            switch ( stepSequence ) {
                case Init:                    
                    // Log << _now << ": Init happened. Step started. Goal: " << 
                    //     posSequence[0][1] << " at time: " << posSequence[0][0] << 
                    //     " Pos: " << commander->getEncoder()->getPosComp() << 
                    //     endl;                    
                    commander->init( posSequence[0][0], posSequence[0][1] ); //Never move more than one lap, for now                       
                    stepSequence = First;
                    break;                
                case First:
                    if ( commander->hasArrived() ) {                        
                        // Log << _now << ": First phase done. Step continued. Goal: " << 
                        //    posSequence[1][1] << " at time: " << posSequence[1][0] << 
                        //    " Pos: " << commander->getEncoder()->getPosComp() << 
                        //    endl;
                        commander->init( posSequence[1][0], posSequence[1][1] ); //Never move more than one lap, for now
                        stepSequence = Second;                                                
                    }
                    break;
                case Second:
                    //Log << "Speed: " << this->commander->getEncoder()->getSpeedCPS() << endl;
                    if ( commander->hasArrived() ) {
                        // Log << _now << ": Second phase done. Step ended." << 
                        //    " Pos: " << commander->getEncoder()->getPosComp() << endl;
                        stepSequence = Stop;
                    }
                    break;
                case Stop:                    
                    break;                
            }
        }

        int* getPosSequence( int i ) { return this->posSequence[i]; } //Only for testing         

        virtual void stop()
        {
            RecurringTaskBase::stop();
            commander->stop();
        }

};

#endif