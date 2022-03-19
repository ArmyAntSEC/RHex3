#pragma once

#include <MotorSpeedRegulatorInterfaces.h>
#include <RunnableInterface.h>
#include <SerialStream.h>

class MotorSpeedCommander: public MotorSpeedCommanderInterface, RunnableInterface
{
private:
    RotationalPositionProvider* currentRotPos;
    SpeedRegulatorInterface* speedRegulator;
    RotationalPosition goalPos;
    uint32_t timeGoalMicros;
    int16_t maxSpeedCPS = 5000;
    bool arrived = false;
    bool isRunning = false;

    long cappedSpeedCPS( long speedCPS )
    {
        if ( speedCPS < 0 || speedCPS > maxSpeedCPS ) {
            return maxSpeedCPS;
        } else {
            return speedCPS;
        }
    }

public:
    MotorSpeedCommander(RotationalPositionProvider* _rotPos, SpeedRegulatorInterface* _speedRegulator):
        currentRotPos(_rotPos), speedRegulator(_speedRegulator)
    {}
    
    void config( int16_t _maxSpeedCPS )
    {        
        maxSpeedCPS = _maxSpeedCPS;
    }

    virtual void setGoal( int16_t _clicks, uint32_t _time )
    {                
        int16_t thisClicks = currentRotPos->getClicks();
        if ( thisClicks > _clicks ) {
            goalPos = RotationalPosition(currentRotPos->getLaps()+1, _clicks);                
        } else {
            goalPos = RotationalPosition(currentRotPos->getLaps(), _clicks);                
        }
        timeGoalMicros = _time;
    }        

    void stop()
    {
        isRunning = false;
    }

    void start()
    {
        isRunning = true;        
    }

    bool getIsRunning()
    {
        return isRunning;
    }

    long computeTargetSpeedCPS( long timeLeftMicros, long clicksLeft )
    {       
        long clicksLeftScaled = 1e3L * clicksLeft;        
        long timeLeftMillis = timeLeftMicros / 1e3;
        if ( timeLeftMillis == 0) { timeLeftMillis = 1; } //Avoid divide by zero;        
        long targetSpeedCPS = clicksLeftScaled / timeLeftMillis;
        long targetSpeedCPSCapped = cappedSpeedCPS( targetSpeedCPS );
        return targetSpeedCPSCapped;
    }

    virtual void run(uint32_t nowMicros)
    {                                                
        if ( isRunning ) {                          
            long clicksLeft = goalPos.getLinearPosition() - currentRotPos->getLinearPosition();                                    
            if ( clicksLeft < 0 )
            {                                
                arrived = true;                
                speedRegulator->setSetPoint( 0 );                                                        
            } else {            
                arrived = false;
                long timeLeft = (int32_t)timeGoalMicros - (int32_t)nowMicros;                                
                long targetSpeed = computeTargetSpeedCPS( timeLeft, clicksLeft );                                                 
                speedRegulator->setSetPoint( targetSpeed );                                                            
            }                  
        }     
           
    }

    bool hasArrived()
    {
        //TODO: Make sure that this value is reset when new position is sent.
        return arrived;
    }
};
