#pragma once

#include <MotorSpeedRegulatorInterfaces.h>
#include <RunnableInterface.h>
#include <SerialStream.h>

class MotorSpeedCommander : public MotorSpeedCommanderInterface, RunnableInterface
{
private:
    RotationalPositionProvider *currentRotPos;
    SpeedRegulatorInterface *speedRegulator;
    RotationalPosition goalPos;
    int32_t timeGoalMicros;
    int16_t maxSpeedCPS = 5000;
    bool arrived = false;
    bool isRunning = false;

    int32_t cappedSpeedCPS(int32_t speedCPS)
    {
        if (speedCPS < 0 || speedCPS > maxSpeedCPS)
        {
            return maxSpeedCPS;
        }
        else
        {
            return speedCPS;
        }
    }

public:
    MotorSpeedCommander(RotationalPositionProvider *_rotPos, SpeedRegulatorInterface *_speedRegulator) : currentRotPos(_rotPos), speedRegulator(_speedRegulator)
    {
    }

    void config(int16_t _maxSpeedCPS)
    {
        maxSpeedCPS = _maxSpeedCPS;
    }

    virtual void setGoal(MotorCommanderGoal goal, int32_t nowMicros)
    {
        goalPos = RotationalPosition(goal.targetPositionClicks);
        timeGoalMicros = nowMicros + goal.targetRelativeTimeMicros;
        start();
    }

    void stop()
    {
        isRunning = false;
        speedRegulator->stop();
    }

    void start()
    {
        isRunning = true;
        speedRegulator->start();
    }

    bool getIsRunning()
    {
        return isRunning;
    }

    int32_t computeTargetSpeedCPS(int32_t timeLeftMicros, int32_t clicksLeft)
    {
        int32_t clicksLeftScaled = 1e3L * clicksLeft;
        int32_t timeLeftMillis = timeLeftMicros / 1e3;
        if (timeLeftMillis == 0)
        {
            timeLeftMillis = 1;
        } // Avoid divide by zero;
        int32_t targetSpeedCPS = clicksLeftScaled / timeLeftMillis;
        int32_t targetSpeedCPSCapped = cappedSpeedCPS(targetSpeedCPS);
        // Log << PRINTVAR(clicksLeft) << PRINTVAR(timeLeftMicros) << PRINTVAR( targetSpeedCPSCapped ) << endl;
        return targetSpeedCPSCapped;
    }

    virtual void run(int32_t nowMicros)
    {
        if (isRunning)
        {
            // Log << "Commander running" << endl;
            int32_t clicksLeft = goalPos.getLinearPosition() - currentRotPos->getLinearPosition();
            if (clicksLeft < 0)
            {
                arrived = true;
                speedRegulator->setSetPoint(0);
                Log << "Arrived!" << endl;
            }
            else
            {
                arrived = false;
                int32_t timeLeft = (int32_t)timeGoalMicros - (int32_t)nowMicros;
                int32_t targetSpeed = computeTargetSpeedCPS(timeLeft, clicksLeft);
                speedRegulator->setSetPoint(targetSpeed);
                // Log << PRINTVAR(timeLeft) << PRINTVAR(targetSpeed) << endl;
            }
        }
    }

    bool hasArrived()
    {
        // TODO: Make sure that this value is reset when new position is sent.
        return arrived;
    }
};
