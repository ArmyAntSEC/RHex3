#pragma once

#include <SpeedComputer.h>

struct HasAnInputDriveSignal
{
    virtual void setInputDriveSignal( int driveSignal )=0;
    virtual int getLastInputDriveSignal()=0;
};

class SpeedRegulator
{
private:
    const int maxOutput = 255;

    bool isOn = false;

    float proportionalTerm;
    float integratorTerm;
    float derivativeTerm;
    int filterLength;

    float input;
    float lastInput;

    float integratorCumulativeValue;

    int setPoint;

    CanProvideSpeed* speedSource;
    HasAnInputDriveSignal* target;
public:
    void config( CanProvideSpeed* _speedSource, 
        HasAnInputDriveSignal* _target, 
        int _P, int _I, int _D, int _filterLength )
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
        integratorCumulativeValue = clampOutput( target->getLastInputDriveSignal() );
    }

    void stop()
    {
        isOn = false;
    }

    void setSetPoint( int _setPoint )
    {
        setPoint = _setPoint;
    }

    void step()
    {
        if ( this->isOn ) {
            int input = speedSource->getSpeedCPS();
            int error = (setPoint - input);
            float proportionalOutput = proportionalTerm * error;
            float derivateiveOutput = derivativeTerm * (input - lastInput); 
            integratorCumulativeValue += integratorTerm * error;            
            int clampedOutput = clampOutput( proportionalOutput + derivateiveOutput + integratorCumulativeValue );
            target->setInputDriveSignal( clampedOutput ); 
        }
    }

    int clampOutput( int output )
    {
        if (output > maxOutput )
            return maxOutput;        
        else if ( output < 1 )
            return 1; //We use 0 to signal hard break. 
        else
            return output;
    }
};