#ifndef _TESTMOVETOPOSITION_H_
#define _TESTMOVETOPOSITION_H_

#include "RemoteRoutine.h"
#include "dataLogger.h"
#include "MotorSpeedRegulator.h"

class TestMoveToPosition: public RemoteRoutine
{
    private:
        long int posToMoveTo; 
        unsigned long int speedToMove;        
        
        DataLogger* logger;
        
        MotorSpeedRegulator* regulator;
        
        int posLogIdx = 0;
        int speedLogIdx = 0;
        int powerLogIdx = 0;
        LOGGABLE( "TstMvPos" );

    public:
        TestMoveToPosition ( HomingEncoder* _encoder, MotorDriver* _driver, 
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

            posLogIdx = logger->registerVariable((char*)"pos");
            speedLogIdx = logger->registerVariable((char*)"speed");
            powerLogIdx = logger->registerVariable((char*)"power");

            DEBUG ( F("Will stop at position ") << this->posToMoveTo );            
        }

        virtual void run( unsigned long int _now )
        {                                    
            long int pos = this->encoder->getPosComp();
            long int speed = encoder->getSpeedCPMS();
            long int power = regulator->getFilteredOutput();
            
            if ( pos > this->posToMoveTo ){
                this->regulator->stop();
                this->stop(); //Stop to avoid hogging resources
                DEBUG( F( "Motor stopped at position " << pos ) );                 
            } else {
                DEBUG ( F("Go") );
            }            
            
            logger->storeValue( posLogIdx, pos );
            logger->storeValue( speedLogIdx, speed );
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
                    this->speedToMove = argumentValue;
                    DEBUG( F("Set speed: ") << this->speedToMove );
                    break;                
                default:
                    DEBUG( F("Unsupported arg number:") << argumentNumber );
            }
        }        
};


#endif