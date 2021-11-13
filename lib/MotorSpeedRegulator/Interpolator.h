#ifndef _INTERPOLATOR_H_
#define _INTERPOLATOR_H_

#include <unity.h>

class InterpolatorInterface
{
    virtual unsigned int doInterpolation(unsigned int x, unsigned int xList[], unsigned int yList[] ) = 0;
};

#include <FixedPointsCommon.h>

class Interpolator
{
    public:
    unsigned int doInterpolation(unsigned int x, unsigned int xList[], unsigned int yList[], int tableLength )
    {        
        //Check if we are out of range
        if (x <= xList[0])
        {
            return yList[0];
        }
        else if (x >= xList[tableLength-1])
        {
            return yList[tableLength-1];
        }

        int xIdx = 0;
        while (xList[xIdx] < x)
        {
            xIdx++;
        }

        int xLow = xList[xIdx - 1];
        int xHigh = xList[xIdx];
        SQ15x16 xSpan = xHigh - xLow;
        int yLow = yList[xIdx - 1];
        int yHigh = yList[xIdx];
        SQ15x16 ySpan = yHigh - yLow;

        SQ15x16 xRem = x - xLow;
        SQ15x16 factor = ySpan / xSpan;
        SQ15x16 yRem = xRem * factor;
        
        int y = roundFixed(yRem).getInteger() + yLow;

        return y;
    }

};


#endif