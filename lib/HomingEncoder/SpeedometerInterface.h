#ifndef _SPEEDOMETERINTERFACE_H_
#define  _SPEEDOMETERINTERFACE_H_

class SpeedometerInterface
{
    public:
        virtual long int getSpeedCPS() = 0;
        virtual long int getSpeedCPSFiltered() = 0;
};

#endif