#pragma once

#include <RotationPositionWithLaps.h>

class HomingEncoderInterface
{
    public:
        virtual long int getSpeedCPS() = 0;
        virtual long int getSpeedCPSFiltered() = 0;
        virtual RotationPositionWithLaps getPosition() = 0;
};
