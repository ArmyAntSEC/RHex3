#pragma once
#include <RunnableInterface.h>
#include <MotorSpeedRegulatorInterfaces.h>

class Homer : public RunnableInterface
{
private:
    MotorDriverInterface* driver;
    LinearPositionProvider* linPos;
public:
    Homer( MotorDriverInterface* _driver, LinearPositionProvider* _linPos ):
        driver(_driver), linPos(_linPos)
    {}

    void run (int32_t _nowMicros)
    {
        if ( linPos->isHomed() ) {
            driver->setMotorPWM(0);    
        }
    }

    void start()
    {
        driver->setMotorPWM(64);
    }
};