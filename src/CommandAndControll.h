#ifndef _COMMANDANDCONTROLL_H_
#define _COMMANDANDCONTROLL_H_

#include <Arduino.h>

#include <RemoteRoutine.h>
#include <TaskScheduler.h>
#include <LevelLogger.h>

class CommandAndControll: public RecurringTaskBase
{
    private:
        LOGGABLE( "Cmnd&Ctrl" );
        static const int MaxRoutines = 16;
        RemoteRoutine* routines[MaxRoutines];             
        byte argumentBuffer[16];
        unsigned int argumentBufferWritePos = 0;
        unsigned int numberOfArgumentBytes = 0;
        int commandInt = -1;

        DataLogger* logger;

    public:
        CommandAndControll( DataLogger* _logger ):
        logger(_logger)
        {            
            memset( this->routines, 0, sizeof(this->routines) );
        }

        void registerRemoteRoutine( RemoteRoutine* _routine, int _command )
        {
            if ( _command < MaxRoutines && routines[_command] == 0 ) {                
                this->routines[_command] = _routine;                
                DEBUG( F("Registered command ") << _command );
            } else {
                DEBUG( F("Could not register command at pos ") << _command );
            }
        }

        void run ( unsigned long int _now )
        {                                                              
            if ( Serial.available() ) {                  
                if ( this->commandInt < 0 ) {
                    unsigned int command = Serial.read();
                    if ( routines[command] != 0 ) {
                        this->commandInt = command;
                        RemoteRoutine* thisCommand = this->routines[commandInt];                    
                        this->numberOfArgumentBytes = thisCommand->getNumberOfArguments()*sizeof(float);   
                        DEBUG( F("Found command byte: ") << this->commandInt );                             
                        DEBUG( F("Requires ") << this->numberOfArgumentBytes << " arguments." );                                                     
                    } else {
                        ERROR ( F( "Command not recognized: ") << command );
                    }
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
                        
                        float * argumentBufferFloat = (float*)argumentBuffer;
                        int argumentFloatLength = argumentBufferWritePos/4;
                        
                        //Reset the logger before we start the next command.
                        logger->reset(_now);

                        thisCommand->parseArgumentsAndInit (                             
                            argumentBufferFloat,
                            argumentFloatLength, 
                            _now );
                        
                        this->commandInt = -1;
                        this->argumentBufferWritePos = 0;                        

                        logger->sendHeaders();
                    }
                }  
            }          
        }
        
};

#endif