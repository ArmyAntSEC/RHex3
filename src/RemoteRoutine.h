#ifndef _REMOTEROUTINE_H_
#define _REMOTEROUTINE_H_

#include <RecurringTask.h>
#include <Arduino.h>
#include <RecurringTask10ms.h>
#include <HomingEncoder.h>
#include "MotorDriver.h"

class RemoteRoutine: public RecurringTask10ms
{           
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
        
        virtual void init( unsigned long int _now )
        {
            RecurringTask10ms::init( _now );            
        }

        void parseArgumentsAndInit( unsigned long int _now )
        {
            DEBUG(F("Waiting for arguments: ") << this->numArguments );
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
                DEBUG( F("Parsed argument ") << i << F(" with value ") << paramvalue );
                this->storeArgument( i, paramValue );
            }            
            DEBUG( F("All aruments parsed") );
            this->init(_now);            
        }        
};


#endif