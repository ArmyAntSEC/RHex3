#pragma once
#include <base.h>

enum LegIdentifiers { FORWARD_LEFT = 0, FORWARD_RIGHT = 1, 
    MIDDLE_LEFT = 2, MIDDLE_RIGHT = 3, 
    BACK_LEFT = 4, BACK_RIGHT = 5 };

struct I2CBase 
{
    static const int LegAddresses[6][2];

    int8_t getControllerIDForLeg(LegIdentifiers leg )
    {
        return LegAddresses[leg][0];
    }

    int8_t getMotorIDForLeg(LegIdentifiers leg )
    {
        return LegAddresses[leg][1];
    }
};
