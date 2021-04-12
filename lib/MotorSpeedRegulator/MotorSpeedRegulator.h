#ifndef _MOTORSPEEDCONTROLLER_H_
#define _MOTORSPEEDCONTROLLER_H_

#include "HomingEncoder.h"
#include "MotorDriver.h"
#include <RecurringTaskBase.h>
#include <SerialStream.h>

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

        int clampOutput( int Output ) {
            if ( Output > 255  ) Output = 255;
            if ( Output < 0 ) Output = 1;      
            return Output;
        }

    public:
        virtual void init()
        {
            RecurringTaskBase::init();  
            Input = encoder->getSpeedCPMS();    
            lastInput = Input;
            ITerm = clampOutput(driver->getMotorPWM());
        }

        virtual void config( HomingEncoder* _encoder, MotorDriver* _driver, 
            float _P, float _D, float _I, float _filter )
        {                        
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

                Output = clampOutput( P*Error + D*dInput + ITerm );
                        
                OutputFiltered = (Output + OutputFiltered*(filter-1))/filter;
                driver->setMotorPWM(OutputFiltered);   
                //Log << "Speed: " << Input << " Power: " << Output << " Error: " << Error << endl;
            }
            
        }

        int getInput() { return this->Input; }
        int getFilteredOutput() { return this->OutputFiltered; }       

};

#endif