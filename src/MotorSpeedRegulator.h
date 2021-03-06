#ifndef _MOTORSPEEDCONTROLLER_H_
#define _MOTORSPEEDCONTROLLER_H_

#include "HomingEncoder.h"
#include "MotorDriver.h"

class MotorSpeedRegulator
{
    private:        
        float P = 0;
        float D = 0;
        float filter = 0;

        int Input = 0;
        long int Output = 0;
        long int OutputFiltered = 0;
        int SetPoint = 0;
        int lastInput = 0;

        HomingEncoder* encoder;
        MotorDriver* driver;

    public:
        virtual void init( HomingEncoder* _encoder, MotorDriver* _driver, 
            float _P, float _D, float _filter )
        {            
            this->encoder = _encoder;
            this->driver = _driver;
            this->P = _P;
            this->D = _D;
            this->filter = _filter;            
        }

        void setSetPoint( int _SetPoint )
        {
            this->SetPoint = _SetPoint;
        }

        virtual void run( ) 
        {            
            Input = encoder->getSpeedCPMS();
            int Error = SetPoint - Input;
            int dInput = Input - lastInput;
            lastInput = Input;

            Output = P*Error + D*dInput;
            if ( Output > 255  ) Output = 255;
            if ( Output < 0 ) Output = 1;      
                    
            OutputFiltered = (Output + OutputFiltered*(filter-1))/filter;
            driver->setMotorPWM(OutputFiltered);   
        }

        int getInput() { return this->Input; }
        int getFilteredOutput() { return this->OutputFiltered; }        
};

#endif