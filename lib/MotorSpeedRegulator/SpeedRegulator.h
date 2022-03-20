#pragma once

#include <SpeedComputer.h>
#include <MotorSpeedRegulatorInterfaces.h>
#include <RunnableInterface.h>

class SpeedRegulator: public SpeedRegulatorInterface, public RunnableInterface
{
private:
    const int16_t maxOutput = 255;

    bool isOn = false;

    float proportionalTerm;
    float integratorTerm;
    float derivativeTerm;
    int16_t filterLength;

    float input;
    float lastInput;

    float integratorCumulativeValue;

    int16_t setPoint;

    CanProvideSpeed* speedSource;
    MotorDriverInterface* target;

    int16_t clampOutput( int16_t output )
    {
        if (output > maxOutput )
            return maxOutput;        
        else if ( output < 1 )
            return 1; //We use 0 to signal hard break. 
        else
            return output;
    }

public:
    void config( CanProvideSpeed* _speedSource, 
        MotorDriverInterface* _target, 
        float _P, float _I, float _D, int16_t _filterLength )
    {
        proportionalTerm = _P;
        integratorTerm = _I;
        derivativeTerm = _D;

        filterLength = _filterLength;

        speedSource = _speedSource;
        target = _target;
    }

    void start()
    {
        isOn = true;
        input = speedSource->getSpeedCPS();
        lastInput = input;
        integratorCumulativeValue = clampOutput( target->getMotorPWM() );
    }

    void stop()
    {
        isOn = false;
    }

    virtual void setSetPoint( int16_t _setPoint )
    {
        setPoint = _setPoint;
    }

    void run( int32_t nowMicros ) 
    {     
        if ( this->isOn ) {                        
            int16_t input = speedSource->getSpeedCPS();
            int16_t error = (setPoint - input);
            float proportionalOutput = proportionalTerm * error;
            float derivateiveOutput = derivativeTerm * (input - lastInput); 
            integratorCumulativeValue += integratorTerm * error;            
            int16_t clampedOutput = clampOutput( proportionalOutput + derivateiveOutput + integratorCumulativeValue );            
            target->setMotorPWM( clampedOutput );             
            //Log << PRINTVAR( input ) << PRINTVAR(setPoint) << PRINTVAR(clampedOutput) << endl;
        }
    }
};