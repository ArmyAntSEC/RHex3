#ifndef _MOTORSCHEDULER_H_
#define _MOTORSCHEDULER_H_

#include "MotorSpeedCommander.h"

class MotorScheduler: public RecurringTaskBase
{
    private:
        MotorSpeedCommander* commander;
        unsigned int schedule[2][2] = {{400,2592}, {400, 1000}};
        unsigned int scheduleIdx = 0;

        unsigned long int targetPos = 0;
        unsigned int loopCount = 0;

    public:
        MotorScheduler( MotorSpeedCommander* _commander ):
            commander( _commander )
        {            
        }

        virtual void init( unsigned long int _now )
        {
            RecurringTaskBase::init();
        }

        virtual void run( unsigned long int _now )
        {
            if ( commander->hasArrived() ) {
                scheduleIdx++;
                if ( scheduleIdx > 1 ) {
                    scheduleIdx = 0;
                    loopCount++;
                }      
                
                if ( loopCount > 10 ) {
                    this->stop();
                }
                
                this->targetPos += schedule[scheduleIdx][1];
                unsigned long int timeToMove = schedule[scheduleIdx][0];
                commander->init(_now, timeToMove, this->targetPos );          
            }
        }
};

#endif