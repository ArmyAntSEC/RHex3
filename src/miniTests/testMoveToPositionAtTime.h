#ifndef _TESTMOVETOPOSITIONATTIME_H_
#define _TESTMOVETOPOSITIONATTIME_H_

#include "RemoteRoutine.h"
#include "dataLogger.h"
#include "MotorSpeedRegulator.h"

class TestMoveToPositionAtTime: public RemoteRoutine
{
    private:
        long int posToMoveTo; 
        long int maxSpeedToMove;        
        unsigned long int timeToMove = 42;        
        unsigned long int timeToArrive;
        
        DataLogger* logger;
        
        MotorSpeedRegulator* regulator;
        
        int posLogIdx = 0;
        int speedLogIdx = 0;
        int powerLogIdx = 0;
        int targetSpeedLogIdx = 0;
        LOGGABLE( "TstMvPosAtTime" );

    public:
        TestMoveToPositionAtTime ( HomingEncoder* _encoder, MotorDriver* _driver, 
            MotorSpeedRegulator* _regulator, DataLogger* _logger ): 
            RemoteRoutine ( 3, _encoder, _driver ), logger(_logger), 
            regulator( _regulator )
        {
        }
        
        virtual void init( unsigned long int _now )
        {            
            RemoteRoutine::init(_now);                        
            this->regulator->start();
            this->timeToArrive = _now + this->timeToMove;            
            DEBUG ( F("Time to move: ") << this->timeToMove << " arrive: " << this->timeToArrive );

            posLogIdx = logger->registerVariable((char*)"pos");
            speedLogIdx = logger->registerVariable((char*)"speed");
            powerLogIdx = logger->registerVariable((char*)"power");
            targetSpeedLogIdx = logger->registerVariable((char*)"targetSpeed");            
            DEBUG ( F("Will stop at position ") << this->posToMoveTo );            
        }

        virtual void run( unsigned long int _now )
        {                                    
            long int pos = this->encoder->getPosComp();
            long int speed = encoder->getSpeedCPS();
            long int power = regulator->getFilteredOutput();
            
            long int clicksLeft = this->posToMoveTo - pos;
            if ( clicksLeft < 0 ) {
                clicksLeft = 0;
            }

            //Compute the speed to move
            long int targetSpeed = 0;
            long int timeLeft = this->timeToArrive - _now;
            if ( timeLeft  < 1 ) {                
                //If we are past due, move at max allowed speed
                targetSpeed = this->maxSpeedToMove; 
            } else {
                targetSpeed = (1000*clicksLeft) / timeLeft; 
            }

            //Convert to clicks per second as timeLeft is in ms.
            
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

        virtual void storeArgument( int argumentNumber, float argumentValue )
        {
            switch ( argumentNumber )
            {
                case 0:
                    this->posToMoveTo = argumentValue;
                    DEBUG( F("Pos to move to: ") << this->posToMoveTo );
                    break;                
                case 1:
                    this->maxSpeedToMove = argumentValue;
                    DEBUG( F("Max speed: ") << this->maxSpeedToMove );
                    break;                
                case 2:
                    this->timeToMove = argumentValue;
                    DEBUG( F("Time to arrive: ") << this->timeToMove );
                    break;                

                default:
                    DEBUG( F("Unsupported arg number:") << argumentNumber );
            }
        }        
};


#endif