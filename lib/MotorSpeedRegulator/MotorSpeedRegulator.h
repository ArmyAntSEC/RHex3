#ifndef _MOTORSPEEDCONTROLLER_H_
#define _MOTORSPEEDCONTROLLER_H_

#include "HomingEncoder.h"
#include "MotorDriver.h"
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

    int maxOutput = 255;

    bool isOn = false;

    boolean hardBreakMode = false;

    HomingEncoderState *encoder;
    MotorDriver *driver;
    SpeedToPowerConverterProduction *converter;

public:
    virtual void init()
    {
        RecurringTaskBase::init();
        input = encoder->getSpeedCPS();
        lastInput = input;
        integratorCumulativeValue = clampOutput( driver->getMotorPWM() );
    }

    virtual void config(HomingEncoderState *_encoder, MotorDriver *_driver,
                        SpeedToPowerConverterProduction *_converter, float _P, float _D, float _I, float _filter)
    {
        this->encoder = _encoder;
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
        this->lastInput = encoder->getSpeedCPS(); //No derivative kick on first iteration
        this->integratorCumulativeValue = 0;
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
        input = encoder->getSpeedCPSFiltered();

        int errorTerm = setPoint - input;
        integratorCumulativeValue += integratorTerm * errorTerm;
        integratorCumulativeValue = this->clampOutputForSpeed(integratorCumulativeValue, setPoint);

        int diffOfInput = input - lastInput;
        lastInput = input;

        output = clampOutputForSpeed(proportionalTerm * errorTerm + derivativeTerm * diffOfInput + integratorCumulativeValue, setPoint);
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