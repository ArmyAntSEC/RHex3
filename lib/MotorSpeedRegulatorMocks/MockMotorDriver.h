#include <MotorDriverInterface.h>

struct MockMotorDriver: public MotorDriverInterface
{
    int motorPWM = 128;

    virtual void setMotorPWM( int _motorPWM )
    {
        motorPWM = _motorPWM;
    }

    virtual int getMotorPWM()
    {
        return motorPWM;
    }
};