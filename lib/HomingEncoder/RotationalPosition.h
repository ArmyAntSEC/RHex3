#pragma once

#include "LinearPositionalEncoder.h"

class RotationalPosition
{
private:
    LinearPositionProvider * linPos;
    static const long clicksPerLapNum = 10775776;
    static const long clicksPerLapDen = 3000;
public:
    RotationalPosition( LinearPositionProvider* _linPos ): linPos(_linPos)
    {}

    long getLaps()
    {
        return 0;
    }

    long getClicks()
    {
        return linPos->getLinearPosition();
    }  
};