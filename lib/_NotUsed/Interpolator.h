#ifndef _INTERPOLATOR_H_
#define _INTERPOLATOR_H_

class Interpolator
{
    public:

    int doInterpolation(int x, int const * xList, int const * yList, int tableLength )
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
        long int xSpan = xHigh - xLow;
        int yLow = yList[xIdx - 1];
        int yHigh = yList[xIdx];
        long int ySpan = yHigh - yLow;

        long int xRem = x - xLow;
        int factorTimes1000 = (1000 * ySpan) / xSpan;
        int yRem = (xRem * factorTimes1000)/1000;
        
        int y = yRem + yLow;

        return y;
    }

};

#endif