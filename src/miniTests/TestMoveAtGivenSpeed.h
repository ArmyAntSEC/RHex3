#ifndef _TESTMOVEATGIVENSPEED_H_
#define _TESTMOVEATGIVENSPEED_H_

#include "MotorSpeedRegulator.h"

class TestMoveAtGivenSpeed: public RemoteRoutine
{
    private:
        unsigned long int timeToMoveSec; 
        unsigned long int speedToMove;
        unsigned long int stopTime;
        
        DataLogger* logger;
        
        MotorSpeedRegulator* regulator;
        
        int posLogIdx = 0;
        int speedLogIdx = 0;
        int powerLogIdx = 0;
        LOGGABLE( "TstMvSpeed" );

    public:
        TestMoveAtGivenSpeed ( HomingEncoder* _encoder, MotorDriver* _driver, 
            MotorSpeedRegulator* _regulator, DataLogger* _logger ): 
            RemoteRoutine ( 2, _encoder, _driver ), logger(_logger), 
            regulator( _regulator )
        {
        }
        
        virtual void init( unsigned long int _now )
        {            
            RemoteRoutine::init(_now);
            this->regulator->start();
            this->regulator->setSetPoint( this->speedToMove );
            this->stopTime = _now + this->timeToMoveSec;         

            posLogIdx = logger->registerVariable((char*)"pos");
            speedLogIdx = logger->registerVariable((char*)"speed");
            powerLogIdx = logger->registerVariable((char*)"power");

            DEBUG ( F("Will stop at time ") << this->stopTime );            
        }

        virtual void run( unsigned long int _now )
        {                                    
            if ( _now > stopTime ){
                this->regulator->stop();
                this->stop(); //Stop to avoid hogging resources
                DEBUG( F( "Motor stopped" ) );                 
            } else {
                DEBUG ( F("Go") );
            }
            long int pos = encoder->getPosComp();
            long int speed = encoder->getSpeedCPMS();
            long int power = regulator->getFilteredOutput();
            logger->storeValue( posLogIdx, pos );
            logger->storeValue( speedLogIdx, speed );
            logger->storeValue( powerLogIdx, power );
        }

        virtual void storeArgument( int argumentNumber, float argumentValue )
        {
            switch ( argumentNumber )
            {
                case 0:
                    this->timeToMoveSec = argumentValue;
                    DEBUG( F("Set time to move: ") << this->timeToMoveSec );
                    break;                
                case 1:
                    this->speedToMove = argumentValue;
                    DEBUG( F("Set speed: ") << this->speedToMove );
                    break;                
                default:
                    DEBUG( F("Unsupported arg number:") << argumentNumber );
            }
        }        
};


#endif