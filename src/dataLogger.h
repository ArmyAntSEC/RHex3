#ifndef _DATALOGGER_H_
#define _DATALOGGER_H_


#include "RecurringTaskBase.h"

/*
//This function is probably overly complicated.
class DataLogger: public RecurringTaskBase
{
    private:
        static const unsigned int MaxColumns = 8;               
        char variableNames[MaxColumns][16];
        float thisRow[MaxColumns];
        bool thisRowWrittenTo = false;
        unsigned int numVariablesRegistered = 0;    
        unsigned long int initTime = 0;
        LOGGABLE( "DataLogger" );    
        const boolean printRaw = true;
    public:
        unsigned int registerVariable( char* variableName )
        {
            int columnNum = -1;
            if ( this->numVariablesRegistered < MaxColumns ) {                
                strncpy( variableNames[numVariablesRegistered], variableName, 16 ); 
                if ( strlen( variableName ) > 15 ) {                    
                    variableNames[numVariablesRegistered][15] = 0; //Make sure to null terminate
                }
                columnNum = numVariablesRegistered;
                numVariablesRegistered++;
            } else {
                ERROR( F("Too many variables registered.") );
            }
            return columnNum;
        }

        void sendHeaders()
        {            
            Log << "time, ";
            for ( unsigned int i = 0; i < numVariablesRegistered-1; i++ ) {
                Log << variableNames[i] << ", ";
            }            
            //Avoid the comma after the last variable name
            Log <<variableNames[numVariablesRegistered-1] << endl;              
        }

        void run( unsigned long int now )
        {                  
            //ERROR ( F("Data logger running") );
            if ( thisRowWrittenTo ) {                
                
                    float fNow = now - this->initTime;
                    Serial.write ( (const byte*)&fNow, sizeof(fNow) );
                    Serial.write( (const byte*)thisRow, numVariablesRegistered*sizeof(float) );                                  
                /*
                    Log << now << ", ";                              
                    for ( unsigned int i = 0; i < numVariablesRegistered; i++ ) {
                        Log << thisRow[i] << ", ";
                    }
                    Log << endl;                    
                * /
                
                memset( thisRow, 0, MaxColumns*sizeof(float) );     
                thisRowWrittenTo = false;     
            }     
            
        }

        void storeValue( int columnIdx, float value )
        {
            thisRow[columnIdx] = value;
            thisRowWrittenTo = true;
        }

        virtual void init( unsigned long int _now )
        {
            RecurringTaskBase::init();
            this->numVariablesRegistered = 0;
            this->thisRowWrittenTo = false;            
            this->initTime = _now;
        }
        
};
*/


#endif