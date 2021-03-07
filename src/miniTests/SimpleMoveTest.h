#ifndef _SIMPLEMOVE_H_
#define _SIMPLEMOVE_H_

#include "RemoteRoutine.h"
#include <HomingEncoder.h>
#include "MotorDriver.h"


class SimpleMoveTest: public RemoteRoutine
{
    private:
        unsigned long int timeToMoveSec; 
        unsigned long int stopTime;
                       
    public:
        SimpleMoveTest(HomingEncoder* _encoder, MotorDriver* _driver ): 
            RemoteRoutine ( 1, _encoder, _driver ) 
        {}

        virtual void run( unsigned long int _now )
        {
            RemoteRoutine::run(_now);
            if ( _now > stopTime ){
                driver->setMotorPWM(0);                
                //Remote Routines should stop when done to 
                //allow new commands to be sent.
                this->stop();
                DEBUG( F("Motor stopped") );                 
            }
        }
        
        virtual void init( unsigned long int _now )
        {
            RemoteRoutine::init (_now );
            driver->setMotorPWM(64);
            this->stopTime = _now + this->timeToMoveSec;
            DEBUG(F("SimpleMoveTest initialized at time ") << _now );
            DEBUG(F("Will stop at ") << this->stopTime );
            this->start(_now);
        }

        virtual void storeArgument( int argumentNumber, float argumentValue )
        {
            switch ( argumentNumber )
            {
                case 0:
                    this->timeToMoveSec = argumentValue;
                    DEBUG( F("Time to move: ") << this->timeToMoveSec );
                    break;                
                default:
                    DEBUG( F("Unsupported arg number:") << argumentNumber );
            }
        }

};

#endif