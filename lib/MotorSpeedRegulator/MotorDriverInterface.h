#ifndef _MOTORDRIVERINTERFACE_H_
#define _MOTORDRIVERINTERFACE_H_

class MotorDriverInterface
{
    public:
        virtual void setMotorPWM( int motorPWM ) = 0;
        virtual int getMotorPWM() = 0;
};

#endif