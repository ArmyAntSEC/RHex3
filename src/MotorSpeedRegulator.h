#ifndef _MOTORSPEEDCONTROLLER_H_
#define _MOTORSPEEDCONTROLLER_H_

#include "HomingEncoder.h"
#include "MotorDriver.h"
#include <RecurringTaskBase.h>
#include "LevelLogger.h"

class MotorSpeedRegulator: public RecurringTaskBase
{
    private:        
        float P = 0;
        float D = 0;
        float I = 0;
        float filter = 0;

        int Input = 0;
        long int Output = 0;
        long int OutputFiltered = 0;
        int SetPoint = 0;
        int lastInput = 0;
        float ITerm = 0;

        int maxOutput = 255;

        bool isOn = false;

        HomingEncoder* encoder;
        MotorDriver* driver;

        LOGGABLE( "SpeedReg" );

    public:
        virtual void init()
        {
            RecurringTaskBase::init();            
            this->stop(); //But we do not start right away            
        }

        virtual void init( HomingEncoder* _encoder, MotorDriver* _driver, 
            float _P, float _D, float _I, float _filter )
        {            
            init();
            this->encoder = _encoder;
            this->driver = _driver;
            this->P = _P;
            this->D = _D;
            this->I = _I;
            this->filter = _filter;                        
        }

        virtual void start() 
        {       
            RecurringTaskBase::start();
            this->isOn = true; 
            this->lastInput = encoder->getSpeedCPMS(); //No derivative kick on first iteration
            this->ITerm = 0;
        }
        
        void stop() 
        {
            RecurringTaskBase::stop();
            this->isOn = false; 
            this->driver->setMotorPWM(0); 
        }

        void setSetPoint( int _SetPoint )
        {
            this->SetPoint = _SetPoint;
        }

        virtual void run( unsigned long int ) 
        {         
            if ( isOn ) {   
                Input = encoder->getSpeedCPMS();
                int Error = SetPoint - Input;
                ITerm += I * Error;
                if ( ITerm > maxOutput ) {
                    ITerm = maxOutput;
                } else if (ITerm < 0 ) {
                    ITerm = 0;
                }

                int dInput = Input - lastInput;
                lastInput = Input;

                Output = P*Error + D*dInput + ITerm;
                if ( Output > 255  ) Output = 255;
                if ( Output < 0 ) Output = 1;      
                        
                OutputFiltered = (Output + OutputFiltered*(filter-1))/filter;
                driver->setMotorPWM(OutputFiltered);   
                DEBUG(F("ITerm: ") << ITerm << " Error: " << Error );
            }
            
        }

        int getInput() { return this->Input; }
        int getFilteredOutput() { return this->OutputFiltered; }        
};

#endif