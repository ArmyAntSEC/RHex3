#ifndef _DATALOGGER_H_
#define _DATALOGGER_H_

#include "LevelLogger.h"
#include "Task.h"

class DataLogger: public Task
{
    private:
        static const unsigned int MaxColumns = 8;               
        char variableNames[MaxColumns][16];
        float thisRow[MaxColumns];
        bool thisRowWrittenTo = false;
        unsigned int numVariablesRegistered = 0;    
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
            Log << variableNames[numVariablesRegistered-1] << endl;            
        }

        void run( unsigned long int now )
        {                  
            if ( thisRowWrittenTo ) {                
                #ifndef DEBUG_LOG // If we are showing debug log data, sending this data makes no sense.
                    float fNow = now;
                    Serial.write ( (const byte*)&fNow, sizeof(fNow) );
                    Serial.write( (const byte*)thisRow, numVariablesRegistered*sizeof(float) );                                  
                #else      
                    Log << now << ", ";                              
                    for ( unsigned int i = 0; i < numVariablesRegistered; i++ ) {
                        Log << thisRow[i] << ", ";
                    }
                    Log << endl;                    
                #endif
                
                memset( thisRow, 0, MaxColumns*sizeof(float) );     
                thisRowWrittenTo = false;     
            }     
            
        }

        void storeValue( int columnIdx, float value )
        {
            thisRow[columnIdx] = value;
            thisRowWrittenTo = true;
        }
        
};



#endif