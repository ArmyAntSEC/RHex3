#ifndef _REMOTEROUTINE_H_
#define _REMOTEROUTINE_H_

#include <RecurringTask.h>
#include <Arduino.h>
#include <HomingEncoder.h>
#include "MotorDriver.h"

class RemoteRoutine: public RecurringTaskBase
{           
    private:
        LOGGABLE( "RemoteRoutine" );
    protected:
        const int numArguments;                            
        HomingEncoder* encoder;
        MotorDriver* driver;

    public:
        RemoteRoutine(int _numArguments, HomingEncoder* _encoder, MotorDriver* _driver ): 
            numArguments(_numArguments), encoder(_encoder), driver(_driver)            
        {
            stop(); //Remote routines do not start in a running state.
        }
        
        virtual void storeArgument( int argumentNumber, float argumentValue ) = 0;
            
        virtual void init ( unsigned long int )
        {
            RecurringTaskBase::init();
        }

        void parseArgumentsAndInit( byte* argumentBytes, unsigned int argumentBytesLen, unsigned long int _now )
        {
            if ( argumentBytesLen >= this->numArguments*sizeof(float) ) {
                float * argumentFloats = (float *)argumentBytes;
                for ( int i = 0; i < this->numArguments; i++ ) {
                    float thisArgument = argumentFloats[i];
                    thisArgument = 1000; //Force override
                    this->storeArgument( i, thisArgument );
                }
                //What should it say here to init the clock properly?
                this->init(_now);
            } else {
                ERROR ( F("Not enough bytes sent to initialize this routine.") );
            }

            ERROR ( F("Initializing routine: ") << this->getName() );                   
        }
     
        int getNumberOfArguments()
        {
            return this->numArguments;
        }
};


#endif