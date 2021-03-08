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
        static const char* getNameImpl() { static const char name[] = "SimpMvTest"; return name; }                    

    public:
        SimpleMoveTest(HomingEncoder* _encoder, MotorDriver* _driver ): 
            RemoteRoutine ( 1, _encoder, _driver ) 
        {}

        virtual void run( unsigned long int _now )
        {
            DEBUG( F("Motor running") );                 
            RemoteRoutine::run(_now);
            if ( _now > stopTime - 100 ) {
                ERROR( F("Speed: ") << encoder->getSpeedCPMS() << " Pos: " << encoder->getPosComp() );
            }
            if ( _now > stopTime ){
                driver->setMotorPWM(0);                
                //Remote Routines should stop when done to 
                //allow new commands to be sent.
                this->stop();
                ERROR( F("Motor stopped") );                 
            }
        }
        
        virtual void init( unsigned long int _now )
        {
            ERROR(F("SimpleMoveTest initialized at time ") << _now );
            RemoteRoutine::init (_now );
            driver->setMotorPWM(64);
            this->stopTime = _now + this->timeToMoveSec;            
            ERROR(F("Will stop at ") << this->stopTime );
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

        virtual const char* getName()
        {
            return SimpleMoveTest::getNameImpl();
        }

};

#endif