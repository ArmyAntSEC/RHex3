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
        static const char* getNameImpl() { static const char name[] = "Command&Ctrl"; return name; }    
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
                ERROR( F("Could not register command at pos ") << _command );
            }
        }

        void run ( unsigned long int _now )
        {
            DEBUG(F("Running"));
            RecurringTask10ms::run(_now);        

            //Check if any commands are still running.
            //For now, we do not support background tasks. Will add that later.
            bool readyToAcceptCommand = true;
            for ( int i = 0; i < this->MaxRoutines; i++ ) {
                if ( this->routines[i] != 0 && this->routines[i]->isRunning() ) {
                    readyToAcceptCommand = false;
                }            
            }

            //Wait for a command to arrive. This blocks the main loop until a command is successfully parsed.
            //In order to support background tasks, this function should be converted to non-blocking.
            if ( readyToAcceptCommand ) {
                ERROR ( F("Waiting for command") );
                float command = 0;
                byte* rawCommand = (byte*)&command;

                for ( unsigned int i = 0; i < sizeof(float); i++ ) {
                    while ( Serial.available() == 0 ) {
                        delay(1);
                    }
                    rawCommand[i] = Serial.read();
                }
                
                int commandInt = (int)command;
                DEBUG ( F("Received command: ") << commandInt );
                if ( commandInt < this->MaxRoutines ) {
                    RemoteRoutine* thisCommand = this->routines[commandInt];
                    if ( thisCommand != 0 ){                                                
                        thisCommand->parseArgumentsAndInit(_now);                        
                    } else {
                        ERROR( F("Selected a null command" ) );
                    }
                } else {
                    ERROR( F("Selected a non-registered command" ) );
                }
                this->init(millis()); //Reset the clock in this one
            } else {
                DEBUG( F("Tasks running. Not doing anything." ) );
            }
            
        }
        
        virtual const char* getName()
        {  
            return CommandAndControll::getNameImpl();            
        }
};

#endif