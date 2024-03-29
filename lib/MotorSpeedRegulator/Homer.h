#pragma once
#include <RunnableInterface.h>
#include <MotorSpeedRegulatorInterfaces.h>

class Homer : public RunnableInterface
{
private:
    MotorDriverInterface* driver;
    LinearPositionProvider* linPos;
    bool isRunning = false;

public:
    Homer( MotorDriverInterface* _driver, LinearPositionProvider* _linPos ):
        driver(_driver), linPos(_linPos)
    {}

    void run (int32_t _nowMicros)
    {        
        if ( isRunning ) {
            if ( linPos->isHomed() ) {            
                driver->setMotorPWM(0);    
                isRunning = false;            
                Log << "Homing complete" << PRINTVAR(linPos->getLinearPosition()) << endl;
            }
        }
    }

    void start()
    {
        Log << "Homing started" << endl;        
        isRunning = true;
        linPos->unHome();
        driver->setMotorPWM(64);        
    }

    void stop()
    {
        isRunning = false;    
    }
};