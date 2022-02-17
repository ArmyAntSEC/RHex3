#pragma once

#include "LinearPositionalEncoder.h"

class RotationalPosition
{
private:
    LinearPositionProvider * linPos;
public:
    RotationalPosition( LinearPositionProvider* _linPos ): linPos(_linPos)
    {}

    long getLaps()
    {
        return -1;
    }

    long getClicks()
    {
        return -1;
    }
    
    long getRemainder()
    {
        return -1;
    }
};