#ifndef _MOTORSPEEDCONTROLLER_H_
#define _MOTORSPEEDCONTROLLER_H_

#include "SpeedometerInterface.h"
#include "MotorDriverInterface.h"
#include <RecurringTaskBase.h>
#include <SpeedToPowerConverter.h>
#include <SerialStream.h>

class MotorSpeedRegulator : public RecurringTaskBase
{
private:
    float proportionalTerm = 0;
    float derivativeTerm = 0;
    float integratorTerm = 0;
    float filter = 0;

    int input = 0;
    long int output = 0;
    int setPoint = 0;
    int lastInput = 0;
    float integratorCumulativeValue = 0;
    int diffOfInput;
    int errorTerm;
    
    int maxOutput = 255;

    bool isOn = false;

    bool hardBreakMode = false;

    SpeedometerInterface *speedometer;
    MotorDriverInterface *driver;
    SpeedToPowerConverter *converter;

public:
    virtual void init()
    {
        RecurringTaskBase::init();
        this->initState();
    }

    virtual void config(SpeedometerInterface *_speedometer, MotorDriverInterface *_driver,
                        SpeedToPowerConverter *_converter, float _P, float _D, float _I, float _filter)
    {
        this->speedometer = _speedometer;
        this->driver = _driver;
        this->converter = _converter;
        this->proportionalTerm = _P;
        this->derivativeTerm = _D;
        this->integratorTerm = _I;
        this->filter = _filter;
    }

    virtual void start()
    {
        RecurringTaskBase::start();
        this->isOn = true;
        this->initState();
    }

    void initState()
    {
        this->input = speedometer->getSpeedCPSFiltered();
        this->lastInput = this->input; //No derivative kick on first iteration
        this->integratorCumulativeValue = clampOutput( driver->getMotorPWM() );
    }

    void stop()
    {
        RecurringTaskBase::stop();
        this->isOn = false;
        this->driver->setMotorPWM(0);
    }

    int clampOutput(int output)
    {
        if (output > maxOutput)
            output = maxOutput;
        if (output < 0)
            output = 1;
        return output;
    }

    int clampOutputForSpeed(int output, int speed)
    {
        if (output > maxOutput)
        {
            output = maxOutput;
        }
        else
        {
            float powerMargin = 0.6;
            int minOutput = converter->GetPowerForFreeSpeed(speed) * powerMargin;
            if (output < minOutput)
            {
                output = minOutput;
            }
        }
        return output;
    }

    void setSetPoint(int _setPoint)
    {
        this->setPoint = _setPoint;
    }

    int getSetPoint()
    {
        return this->setPoint;
    }

    void useHardBreaks()
    {
        this->hardBreakMode = true;
    }

    void doCorePIDAlgorithmStepClampedForSpeed()
    {
        input = speedometer->getSpeedCPSFiltered();

        errorTerm = setPoint - input;
        integratorCumulativeValue += integratorTerm * errorTerm;
        integratorCumulativeValue = this->clampOutputForSpeed(integratorCumulativeValue, setPoint);

        diffOfInput = input - lastInput;
        lastInput = input;

        int rawOutput = proportionalTerm * errorTerm + derivativeTerm * diffOfInput + integratorCumulativeValue;
        output = clampOutputForSpeed(rawOutput, setPoint);
    }

    void handleHardBreak()
    {
        if (this->hardBreakMode)
        {
            if (input > this->setPoint)
            {
                integratorCumulativeValue = 0;
                output = 0;
                lastInput = input;
            }
            else
            {
                this->hardBreakMode = false;
                lastInput = input;
                integratorCumulativeValue = this->converter->GetPowerForFreeSpeed(input);
            }
        }
    }
    virtual void run(unsigned long int)
    {
        if (isOn)
        {
            doCorePIDAlgorithmStepClampedForSpeed();

            handleHardBreak();

            driver->setMotorPWM(output);
        }
    }

    int getInput()
    {
        return this->input;
    }

    int getOutput()
    {

        return this->output;
    }
};

#endif