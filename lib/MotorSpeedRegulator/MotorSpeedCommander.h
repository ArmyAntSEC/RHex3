#ifndef _MOTORSPEEDCOMMANDER_H_
#define _MOTORSPEEDCOMMANDER_H_

#include <MotorSpeedRegulator.h>
#include <RecurringTaskBase.h>
#include <SerialStream.h>

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
    
    public:        
        virtual void init( unsigned long int _now, unsigned long int _timeToMove, 
                            unsigned long int _posToMoveTo )
        {
            RecurringTaskBase::init();

            this->timeToArrive = _now + _timeToMove;            
            this->posToMoveTo = _posToMoveTo;
            this->hasArrivedAtPos = false;

            this->regulator->init();
        }   
        
        virtual void config(  HomingEncoder* _encoder, MotorDriver* _driver, 
                            MotorSpeedRegulator* _regulator )
        {                    
            this->encoder = _encoder;
            this->driver = _driver;
            this->regulator = _regulator;                    
        }

        virtual void run( unsigned long int _now )
        {                                    
            long int pos = this->encoder->getPosComp();                        
            
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

            //Log << pos << ", " << clicksLeft << ", " << timeLeft << ", " << targetSpeedClamped << endl;

            if ( this->hasArrivedAtPos ){
                this->regulator->stop();
                this->stop(); //Stop to avoid hogging resources                
            } else {
                this->regulator->setSetPoint( targetSpeedClamped );
            }            
                        
        }
        
        boolean hasArrived()
        {
            return this->hasArrivedAtPos;
        }
};
#endif