#ifndef _REMOTEROUTINE_H_
#define _REMOTEROUTINE_H_

#include <RecurringTask.h>
#include <Arduino.h>
#include <RecurringTask10ms.h>
#include <HomingEncoder.h>
#include "MotorDriver.h"

class RemoteRoutine: public RecurringTask10ms
{           
    private:
        static const char* getNameImpl() { static const char name[] = "RemoteRoutine"; return name; }    
    protected:
        const int numArguments;                            
        HomingEncoder* encoder;
        MotorDriver* driver;

    public:
        RemoteRoutine(int _numArguments, HomingEncoder* _encoder, MotorDriver* _driver ): 
            numArguments(_numArguments), encoder(_encoder), driver(_driver)            
        {
            RecurringTask::stop(); //Remote routines do not start in a running state.
        }
        
        virtual void storeArgument( int argumentNumber, float argumentValue ) = 0;
            
        void parseArgumentsAndInit( byte* argumentBytes, unsigned int argumentBytesLen, unsigned long int _now )
        {
            if ( argumentBytesLen >= this->numArguments*sizeof(float) ) {
                float * argumentFloats = (float *)argumentBytes;
                for ( int i = 0; i < this->numArguments; i++ ) {
                    this->storeArgument( i, argumentFloats[i] );
                }
            } else {
                ERROR ( F("Not enough bytes sent to initialize this routine.") );
            }

            ERROR ( F("Initializing routine: ") << this->getName() );

            this->init(millis());            
        }
     
        int getNumberOfArguments()
        {
            return this->numArguments;
        }

        virtual const char* getName()
        {
            return RemoteRoutine::getNameImpl();
        }        
};


#endif