#include <HomingEncoderInterface.h>

struct MockHomingEncoder: public HomingEncoderInterface
{    
    int speedCPS = 3;
    int speedCPSFiltered = 5;
    RotationPositionWithLaps pos;    
    virtual long int getSpeedCPS() { return speedCPS; }
    virtual long int getSpeedCPSFiltered() { return speedCPSFiltered; }
    virtual RotationPositionWithLaps getPosition() { return pos; }
};