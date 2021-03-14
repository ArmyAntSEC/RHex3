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
            if ( _now > stopTime ){
                driver->setMotorPWM(0);                                
                this->stop(); //Stop to avoid hogging resources
                ERROR( F("Motor stopped") );                 
            }
        }
        
        virtual void init( unsigned long int _now )
        {
            ERROR(F("SimpleMoveTest initialized at time ") << _now );
            RemoteRoutine::init(_now);
            driver->setMotorPWM(128);
            this->stopTime = _now + this->timeToMoveSec;            
            ERROR(F("Will stop at ") << this->stopTime );            
        }

        virtual void storeArgument( int argumentNumber, float argumentValue )
        {
            switch ( argumentNumber )
            {
                case 0:
                    this->timeToMoveSec = 1000; //argumentValue;
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