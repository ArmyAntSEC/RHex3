#ifndef _SPEEDTOPOWERCONVERTER_H_
#define _SPEEDTOPOWERCONVERTER_H_

#include <FixedPointsCommon.h>
#include <EEPROMStorage.h>
#include <LevelLogger.h>

class SpeedToPowerConverter
{

public:
    static const int tableLength = 7;
    static const int tableWidth = 2;
    
    //We default to the end of the EEPROM which we use for testing to not accidentally
    //overwrite the measured values at the start of the EEPROM array.
    unsigned int eepromStorageStartIndex = 96; 
    unsigned int speedVsPower[tableWidth][tableLength] = {{20, 24, 32, 48, 64, 128, 255}, {789, 1363, 2145, 3472, 4507, 6509, 7735}};

    void setEEPROMStartIndex( int startIndex )
    {
        this->eepromStorageStartIndex = startIndex;
    }

    void initFromEEPROM()
    {                        
        int* speedVsPowerAddress = (int*)speedVsPower;
        EEPROMStorage::readIntArrayFromAddress( eepromStorageStartIndex, speedVsPowerAddress, tableLength*tableWidth );        
    }

    void saveToEEPROM()
    {                        
        int* speedVsPowerAddress = (int*)speedVsPower;
        EEPROMStorage::writeIntArrayToAddress( eepromStorageStartIndex, speedVsPowerAddress, tableLength*tableWidth );        
    }
    
    unsigned int doInterpolation(unsigned int x, unsigned int yList[], unsigned int xList[] )
    {        
        //Check if we are out of range
        if (x <= xList[0])
        {
            return yList[0];
        }
        else if (x >= xList[6])
        {
            return yList[6];
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
        /*        
        Log << "Interpolating to: " << speed << endl;
        Log << speedLow << ", " << speedHigh << ", " << yLow << ", " << yHigh << endl;
        Log << speedSpan.getInteger() << ", " << ySpan.getInteger() << endl;
        Log << (double)xRem << ", " << (double)factor << ", " << (double)yRem << endl; 
        Log << y << endl;            
        */
        return y;
    }

    unsigned int GetPowerForFreeSpeed(unsigned int speed) {
        return doInterpolation(speed, speedVsPower[0], speedVsPower[1] );
    }  

    void setPowerAndSpeedPair( int index, int power, int speed )
    {
        speedVsPower[0][index] = power;
        speedVsPower[1][index] = speed;
    }
};

#endif