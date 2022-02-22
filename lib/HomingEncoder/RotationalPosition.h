#pragma once

#include "HomingEncoderInterfaces.h"

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
        long long clickPosRaw = linPos->getLinearPosition();
        return clickPosRaw * clicksPerLapDen / clicksPerLapNum;
    }

    long getClicks()
    {
        long long laps = getLaps();
        long long clickPosRaw = linPos->getLinearPosition();
        long clicksRemain = clickPosRaw - laps * clicksPerLapNum / clicksPerLapDen;
        return clicksRemain;
    }  
};