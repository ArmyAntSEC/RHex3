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
            
        void parseArgumentsAndInit( unsigned long int _now )
        {
            DEBUG(F("RR: Waiting for arguments: ") << this->numArguments );
            for ( int i = 0; i < this->numArguments; i++ )
            {
                float paramValue = 0;
                byte* rawParam = (byte*)&paramValue;
                for ( unsigned int i = 0; i < sizeof(float); i++ ) {
                    while ( Serial.available() == 0 ) {
                        delay(1);
                    }
                    rawParam[i] = Serial.read();
                }
                paramValue = 1000; //Force override
                DEBUG( F("RR: Parsed argument ") << i << F(" with value ") << paramValue );
                this->storeArgument( i, paramValue );
            }            
            DEBUG( F("RR: All aruments parsed") );
            //The above can have taken a lot of time, so we get a fresh timestamp here.
            this->init(millis());            
        }
     
        virtual const char* getName()
        {
            return RemoteRoutine::getNameImpl();
        }        
};


#endif