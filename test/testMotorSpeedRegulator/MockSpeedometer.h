#include <SpeedometerInterface.h>

struct MockSpeedometer: public SpeedometerInterface
{    
    virtual long int getSpeedCPS() { return 3; }
    virtual long int getSpeedCPSFiltered() { return 5; }
};