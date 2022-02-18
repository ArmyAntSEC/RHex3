#include <SpeedometerInterface.h>

struct MockSpeedometer: public SpeedometerInterface
{    
    virtual  int getSpeedCPS() { return 3; }
};