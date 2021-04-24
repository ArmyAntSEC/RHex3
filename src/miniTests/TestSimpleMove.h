#ifndef _SIMPLEMOVE_H_
#define _SIMPLEMOVE_H_

#include "RecurringTaskBase.h"
#include <HomingEncoder.h>
#include "MotorDriver.h"
#include "dataLogger.h"

class TestSimpleMove: public RecurringTaskBase
{
    private:
        unsigned long int timeToMove; 
        unsigned long int stopTime;

        HomingEncoder* encoder;
        MotorDriver* driver;    
        DataLogger* logger;

        int posLogIdx = 0;
        int speedLogIdx = 0;
        int powerLogIdx = 0;
        LOGGABLE( "SimpMvTst" );

    public:
        TestSimpleMove(HomingEncoder* _encoder, MotorDriver* _driver, DataLogger* _logger ): 
            encoder(_encoder), driver(_driver), logger(_logger)
        {   
            timeToMove = 1000;                     
        }

        virtual void run( unsigned long int _now )
        {                                    
            if ( _now > stopTime ){
                driver->setMotorPWM(0);                                
                this->stop(); //Stop to avoid hogging resources
                //ERROR( F( "Motor stopped" ) );                 
            }
            long int pos = encoder->getPosComp();
            long int speed = encoder->getSpeedCPS();
            logger->storeValue(posLogIdx, pos );
            logger->storeValue(speedLogIdx, speed );
            logger->storeValue(powerLogIdx, 128 );
            //ERROR ( F("Motor running" ))
        }
        
        virtual void init( unsigned long int _now )
        {
            DEBUG(F("SimpleMoveTest initialized at time ") << _now );
            RecurringTaskBase::init();
            driver->setMotorPWM(128);
            this->stopTime = _now + this->timeToMove;   
            
            //Register two variables with the logger.
            posLogIdx = logger->registerVariable((char*)"pos");
            speedLogIdx = logger->registerVariable((char*)"speed");
            powerLogIdx = logger->registerVariable((char*)"power");            
         
            //ERROR(F("Will stop at ") << this->stopTime << " Is running: " << this->isRunning() );                        
        }
};

#endif