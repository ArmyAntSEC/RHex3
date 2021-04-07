#ifndef _MOTORSPEEDCOMMANDER_H_
#define _MOTORSPEEDCOMMANDER_H_

#include <MotorSpeedRegulator.h>
#include <RecurringTaskBase.h>
#include "LevelLogger.h"
#include "dataLogger.h"

class MotorSpeedCommander: public RecurringTaskBase
{
    private:
        long int posToMoveTo; 
        long int maxSpeedToMove = 8000;       
        unsigned long int timeToMove;        
        unsigned long int timeToArrive;
        boolean hasArrivedAtPos = false;
                        
        HomingEncoder* encoder;
        MotorDriver* driver;
        
        MotorSpeedRegulator* regulator;
        
        DataLogger* logger;

        int posLogIdx = 0;
        int speedLogIdx = 0;
        int powerLogIdx = 0;
        int targetSpeedLogIdx = 0;
        LOGGABLE( "MtrSpeedCmdr" );

    public:
        MotorSpeedCommander ( HomingEncoder* _encoder, MotorDriver* _driver, 
            MotorSpeedRegulator* _regulator, DataLogger* _logger ): 
            encoder( _encoder ), driver( _driver ), regulator( _regulator ), 
            logger(_logger)            
        {
        }

        void registerVariables()
        {
            posLogIdx = logger->registerVariable((char*)"pos");
            speedLogIdx = logger->registerVariable((char*)"speed");
            powerLogIdx = logger->registerVariable((char*)"power");
            targetSpeedLogIdx = logger->registerVariable((char*)"targetSpeed");            
            DEBUG ( F("Will stop at position ") << this->posToMoveTo );            
        }        
        
        virtual void init( unsigned long int _now, unsigned long int _timeToMove, 
            unsigned long int _posToMoveTo )
        {            
            RecurringTaskBase::init();

            this->regulator->start();
            this->timeToArrive = _now + _timeToMove;            
            this->posToMoveTo = _posToMoveTo;
            DEBUG ( F("Time to move: ") << _timeToMove << " arrive: " << this->timeToArrive );

            this->hasArrivedAtPos = false;
        }

        virtual void run( unsigned long int _now )
        {                                    
            long int pos = this->encoder->getPosComp();
            long int speed = encoder->getSpeedCPMS();
            long int power = regulator->getFilteredOutput();
            
            long int clicksLeft = this->posToMoveTo - pos;
            if ( clicksLeft < 0 ) {
                clicksLeft = 0;
                this->hasArrivedAtPos = true;
            }

            //Compute the speed to move
            long int targetSpeed = 0;
            long int timeLeft = this->timeToArrive - _now;
            if ( timeLeft  < 1 ) {                
                //If we are past due, move at max allowed speed
                targetSpeed = this->maxSpeedToMove; 
            } else {
                // Speed is in clicks/microsecond, time is in milliseconds.
                targetSpeed = (1000*clicksLeft) / timeLeft; 
            }            
            
            long int targetSpeedClamped = targetSpeed;
            if ( targetSpeedClamped > this->maxSpeedToMove ) {
                targetSpeedClamped = this->maxSpeedToMove;
            }

            if ( pos > this->posToMoveTo ){
                this->regulator->stop();
                this->stop(); //Stop to avoid hogging resources
                DEBUG( F( "Motor stopped at position ") << pos );                 
            } else {
                this->regulator->setSetPoint( targetSpeedClamped );
                DEBUG( F("Speed: ") << targetSpeedClamped )
            }            
            
            logger->storeValue( posLogIdx, pos );
            logger->storeValue( speedLogIdx, speed );
            logger->storeValue( targetSpeedLogIdx, targetSpeed );
            logger->storeValue( powerLogIdx, power );
        }
        
        boolean hasArrived()
        {
            return this->hasArrivedAtPos;
        }
};
#endif