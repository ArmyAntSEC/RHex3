#ifndef _TESTMOVERAMP_H_
#define _TESTMOVERAMP_H_

#include "RecurringTaskBase.h"
#include <HomingEncoder.h>
#include "MotorDriver.h"
#include "dataLogger.h"

class TestMoveRamp: public RecurringTaskBase
{
    private:
        unsigned long int timeToMove; 
        unsigned long int stopTime;

        static const int powerArrayLength = 10;
        int powerArray[powerArrayLength] = {255, 192, 128, 64, 48, 32, 28, 24, 20, 16 };
        int powerArrayIdx = 0;

        HomingEncoder* encoder;
        MotorDriver* driver;    
        DataLogger* logger;

        int posLogIdx = 0;
        int speedLogIdx = 0;
        int powerLogIdx = 0;
        LOGGABLE( "SimpMvTst" );

    public:
        TestMoveRamp(HomingEncoder* _encoder, MotorDriver* _driver, DataLogger* _logger ): 
            encoder(_encoder), driver(_driver), logger(_logger)
        {   
            timeToMove = 1000;                     
        }

        virtual void run( unsigned long int _now )
        {                                    
            
            if ( _now > stopTime ){
                if ( powerArrayIdx >= powerArrayLength ) {
                    driver->setMotorPWM(0);                                
                    this->stop(); //Stop to avoid hogging resources                 
                    //ERROR( F("Motor stopped") );
                } else {
                    powerArrayIdx++;
                    driver->setMotorPWM(powerArray[powerArrayIdx]);
                    this->stopTime = _now + this->timeToMove;   
                    //ERROR ( F("Speed changed: ") << powerArray[powerArrayIdx] );
                }
            }

            long int pos = encoder->getPosComp();
            long int speed = encoder->getSpeedCPMS();
            logger->storeValue(posLogIdx, pos );
            logger->storeValue(speedLogIdx, speed );
            logger->storeValue(powerLogIdx, powerArray[powerArrayIdx] );            
        }
        
        virtual void init( unsigned long int _now )
        {
            DEBUG(F("SimpleMoveTest initialized at time ") << _now );
            RecurringTaskBase::init();
            driver->setMotorPWM(powerArray[0]);
            this->stopTime = _now + this->timeToMove;   
            
            //Register two variables with the logger.
            posLogIdx = logger->registerVariable((char*)"pos");
            speedLogIdx = logger->registerVariable((char*)"speed");
            powerLogIdx = logger->registerVariable((char*)"power");            
         
            //ERROR(F("Will stop at ") << this->stopTime << " Is running: " << this->isRunning() );                        
        }
};

#endif