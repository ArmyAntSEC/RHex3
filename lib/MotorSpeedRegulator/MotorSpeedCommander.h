#ifndef _MOTORSPEEDCOMMANDER_H_
#define _MOTORSPEEDCOMMANDER_H_

#include <MotorSpeedRegulator.h>
#include <RecurringTaskBase.h>
#include <SerialStream.h>

class MotorSpeedCommander: public RecurringTaskBase
{
    private:
        int posToMoveTo;
        int lapsToMove;
        long int maxSpeedToMove = 8000;       
        unsigned long int timeToMove;        
        unsigned long int timeToArrive;
        boolean hasArrivedAtPos = false;
        boolean onSameLapAsTarget = false;
                        
        HomingEncoder* encoder;
        MotorDriver* driver;
        
        MotorSpeedRegulator* regulator;
    
        int computeTargetSpeed( long int timeLeft, long int clicksLeft, int maxSpeed )
        {
            //Compute the speed to move
            long int targetSpeed = 0;            
            if ( timeLeft  < 1 ) {                
                //If we are past due, move at max allowed speed
                targetSpeed = maxSpeed; 
            } else {
                // Speed is in clicks/microsecond, time is in milliseconds.
                targetSpeed = (1000*clicksLeft) / timeLeft; 
            }            

            if ( targetSpeed > this->maxSpeedToMove ) {
                targetSpeed = this->maxSpeedToMove;
            } else if ( targetSpeed <= 0 ) {
                targetSpeed = 1; //If we have moved too far, just coast.
            }
            
            return targetSpeed;
        }

    public:        
        virtual void init( unsigned long int _timeToMoveTo, 
                            int _posToMoveTo )
        {
            RecurringTaskBase::init();            
            this->timeToArrive =  _timeToMoveTo;            
            this->posToMoveTo = _posToMoveTo;            
            this->hasArrivedAtPos = false;
            
            long int pos = this->encoder->getPosComp();                        
            if ( pos > this->posToMoveTo ) {
                this->onSameLapAsTarget = false;
            } else {
                this->onSameLapAsTarget = true;
            }

            this->regulator->init();
            this->regulator->doHardBreak();
                        
            /*
            int newTargetSpeed = this->computeTargetSpeed( _timeToMoveTo - millis(),
                HomingEncoder::positionPositiveDifference( _posToMoveTo, pos ), 
                this->maxSpeedToMove );
            Log << "Old setPoint: " << regulator->getSetPoint() << " New Setpoint: " << newTargetSpeed << endl;
            if ( regulator->getSetPoint() > newTargetSpeed )
            {            
                this->regulator->doHardBreak();
            } 
            */           
            
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
            //TODO: Rewrite this code. It is ugly as what.
            long int pos = this->encoder->getPosComp();                        
            if ( !this->onSameLapAsTarget && pos < this->posToMoveTo ) {
                //Detect that we have now moved to be on the same lap as the target
                this->onSameLapAsTarget = true; //We are now on the same lap as the target
            }
            long int clicksLeft = 0;
            if ( !this->onSameLapAsTarget ) {                
                clicksLeft = encoder->positionPositiveDifference( this->posToMoveTo, pos );
            } else {
                clicksLeft = this->posToMoveTo - pos;
            }
            //End the rewrite here.
            
            if ( clicksLeft < 0 ) {
                clicksLeft = 0;
                this->hasArrivedAtPos = true;
            }
            
            long int targetSpeedClamped = this->computeTargetSpeed ( 
                    this->timeToArrive - _now, clicksLeft, this->maxSpeedToMove );
            /*
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
            
            if ( targetSpeedClampedNew != targetSpeedClamped ) {
                Log << "New algo: " << targetSpeedClampedNew << " old algo: " << targetSpeedClamped << endl;
            }
            */
            
            //Log << millis() << " Pos: " << pos << ", " << clicksLeft << ", " << timeLeft << ", " << targetSpeedClamped << endl;

            if ( this->hasArrivedAtPos ){
                //this->regulator->stop();
                //this->stop(); //Stop to avoid hogging resources                
            } else {
                this->regulator->setSetPoint( targetSpeedClamped );
            }            
                        
        }
        
        boolean hasArrived()
        {
            return this->hasArrivedAtPos;
        }

        virtual void stop()
        {
            RecurringTaskBase::stop();
            regulator->stop();
        }

        HomingEncoder* getEncoder()
        {
            return this->encoder;
        }
};
#endif