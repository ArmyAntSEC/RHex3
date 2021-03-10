#ifndef _COMMANDANDCONTROLL_H_
#define _COMMANDANDCONTROLL_H_

#include <Arduino.h>

#include <RemoteRoutine.h>
#include <TaskScheduler.h>
#include <RecurringTask10ms.h>

class CommandAndControll: public RecurringTask10ms
{
    private:
        static const int MaxRoutines = 16;
        RemoteRoutine* routines[MaxRoutines];
        TaskScheduler* scheduler;        
        static const char* getNameImpl() { static const char name[] = "Comnd&Ctrl"; return name; }    
        byte argumentBuffer[16];
        unsigned int argumentBufferWritePos = 0;
        unsigned int numberOfArgumentBytes = 0;
        int commandInt = -1;

    public:
        CommandAndControll( TaskScheduler* _scheduler )
        {
            this->scheduler = _scheduler;
            memset( this->routines, 0, sizeof(this->routines) );
        }

        void registerRemoteRoutine( RemoteRoutine* _routine, int _command )
        {
            if ( _command < MaxRoutines && routines[_command] == 0 ) {                
                this->routines[_command] = _routine;
                this->scheduler->add( _routine );                
                DEBUG( F("Registerd command ") << _command );
            } else {
                DEBUG( F("Could not register command at pos ") << _command );
            }
        }

        void run ( unsigned long int _now )
        {            
            RecurringTask10ms::run(_now);        
            
            if ( Serial.available() ) {                
                if ( this->commandInt < 0 ) {
                    this->commandInt = Serial.read();
                    
                    this->commandInt = 0; //Hard-code for now
                    
                    RemoteRoutine* thisCommand = this->routines[commandInt];                    
                    this->numberOfArgumentBytes = thisCommand->getNumberOfArguments()*sizeof(float);   
                    DEBUG( F("Found command byte: ") << this->commandInt );                             
                    DEBUG( F("Requires ") << this->numberOfArgumentBytes << " arguments." );                             
                } else {                    
                    byte argumentByte = Serial.read();
                    this->argumentBuffer[this->argumentBufferWritePos++] = argumentByte;
                
                    DEBUG( F("Found argument byte: ") << argumentByte << 
                        " for pos: " << this->argumentBufferWritePos <<
                        " of " << this->numberOfArgumentBytes );

                    if ( this->argumentBufferWritePos == this->numberOfArgumentBytes ) {
                        DEBUG( F("Got all arguments") );
                        //We have got everything we need. Start the command
                        RemoteRoutine* thisCommand = this->routines[this->commandInt];
                        
                        thisCommand->parseArgumentsAndInit (                             
                            this->argumentBuffer, 
                            this->argumentBufferWritePos, 
                            _now );
                        
                        this->commandInt = -1;
                        this->argumentBufferWritePos = 0;                        
                    }
                }  
            }          
        }
        
        virtual const char* getName()
        {  
            return CommandAndControll::getNameImpl();            
        }
};

#endif