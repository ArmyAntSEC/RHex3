#ifndef _COMMANDANDCONTROLL_H_
#define _COMMANDANDCONTROLL_H_

#include <Arduino.h>

#include <RemoteRoutine.h>
#include <TaskScheduler.h>

class CommandAndControll
{
    private:
        static const int MaxRoutines = 16;
        RemoteRoutine* routines[MaxRoutines];
        TaskScheduler* scheduler;        
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
            }
        }

        void run ( unsigned long int _now )
        {
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
                float command = 0;
                byte* rawCommand = (byte*)&command;

                for ( unsigned int i = 0; i < sizeof(float); i++ ) {
                    while ( Serial.available() == 0 ) {
                        delay(1);
                    }
                    rawCommand[i] = Serial.read();
                }
                
                int commandInt = (int)command;
                if ( commandInt < this->MaxRoutines ) {
                    RemoteRoutine* thisCommand = this->routines[commandInt];
                    if ( thisCommand != 0 ){
                        thisCommand->parseArgumentsAndInit(_now);                        
                    }
                }
            }
        }
};

#endif