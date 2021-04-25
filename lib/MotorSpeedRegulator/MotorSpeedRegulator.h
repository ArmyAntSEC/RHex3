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

        int clampOutputForSpeed( int Output, int speed ) {
            if ( Output > 255  ) 
            {
                Output = 255;
            } else {
                int MinOutput = GetPowerForFreeSpeed( speed )*0.6; //We remove 10%
                if ( Output < MinOutput ) {
                    Output = MinOutput;
                }
            }
            return Output;
        }

    public:
        virtual void init()
        {
            RecurringTaskBase::init();  
            Input = encoder->getSpeedCPS();    
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
            this->lastInput = encoder->getSpeedCPS(); //No derivative kick on first iteration
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
            //Log << "Set stepoint: " << _SetPoint << endl;
        }

        virtual void run( unsigned long int ) 
        {         
            if ( isOn ) {   
                Input = encoder->getSpeedCPS();
                int Error = SetPoint - Input;
                ITerm += I * Error;
                ITerm = this->clampOutputForSpeed( ITerm, SetPoint );

                int dInput = Input - lastInput;
                lastInput = Input;

                Output = clampOutputForSpeed( P*Error + D*dInput + ITerm, SetPoint );
                        
                OutputFiltered = (Output + OutputFiltered*(filter-1))/filter;
                driver->setMotorPWM(OutputFiltered);   
                //Log << "Speed: " << Input << " Power: " << Output << " Error: " << Error << endl;
            }
            
        }

        int getInput() { return this->Input; }
        int getFilteredOutput() { return this->OutputFiltered; }       
        
        static unsigned int GetPowerForFreeSpeed( unsigned int speed )
        {

            static const unsigned int speedVsPower[7][2] = {{20, 789}, {24,1363}, {32,2145}, {48,3472}, {64,4507}, {128, 6509}, {255,7735}};    
            
            //Check if we are out of range
            if ( speed <= speedVsPower[0][1] ) {
                return speedVsPower[0][0];
            } else if ( speed >= speedVsPower[6][1] ) {
                return speedVsPower[6][0];
            }

            int speedIdx = 0;    
            while ( speedVsPower[speedIdx][1] < speed ) {
                speedIdx++;
            }
                    
            int speedLow = speedVsPower[speedIdx-1][1];
            int speedHigh = speedVsPower[speedIdx][1];
            SQ15x16 speedSpan = speedHigh - speedLow;
            int powerLow = speedVsPower[speedIdx-1][0];
            int powerHigh = speedVsPower[speedIdx][0];
            SQ15x16 powerSpan = powerHigh - powerLow;
                
            SQ15x16 speedRem = speed - speedLow;
            SQ15x16 factor = powerSpan / speedSpan;
            SQ15x16 powerRem = speedRem * factor;

            int power = roundFixed(powerRem).getInteger() + powerLow;
            /*
            Log << "Interpolating to: " << speed << endl;
            Log << speedLow << ", " << speedHigh << ", " << powerLow << ", " << powerHigh << endl;
            Log << speedSpan.getInteger() << ", " << powerSpan.getInteger() << endl;
            Log << (double)speedRem << ", " << (double)factor << ", " << (double)powerRem << endl; 
            Log << power << endl;
            */
        
            return power;
        }

};

#endif