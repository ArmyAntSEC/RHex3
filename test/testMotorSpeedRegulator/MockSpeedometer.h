#include <SpeedometerInterface.h>

struct MockSpeedometer: public SpeedometerInterface
{    
    int speedCPS = 3;
    int speedCPSFiltered = 5;
    virtual long int getSpeedCPS() { return speedCPS; }
    virtual long int getSpeedCPSFiltered() { return speedCPSFiltered; }
};