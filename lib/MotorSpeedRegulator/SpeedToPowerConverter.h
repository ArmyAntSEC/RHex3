#ifndef _SPEEDTOPOWERCONVERTER_H_
#define _SPEEDTOPOWERCONVERTER_H_

#include <FixedPointsCommon.h>
#include <EEPROMStorage.h>
#include <LevelLogger.h>

/* Measured values on the test motor
Power: 32 Speed: 977
Power: 48 Speed: 1631
Power: 64 Speed: 2142
Power: 96 Speed: 2844
Power: 128 Speed: 3252
Power: 192 Speed: 3652
Power: 255 Speed: 3994
*/

class SpeedToPowerConverterTest
{

public:
    static const int tableLength = 8;
    static const int tableWidth = 2;
    
    //We default to the end of the EEPROM which we use for testing to not accidentally
    //overwrite the measured values at the start of the EEPROM array.
    unsigned int eepromStorageStartIndex = 96; 
    unsigned int speedVsPower[tableWidth][tableLength] = {{0, 20, 24, 32, 48, 64, 128, 255}, {0, 789, 1363, 2145, 3472, 4507, 6509, 7735}};

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

    void printContent()
    {
        Log << "Power vs Speed: " << endl;
        for ( int i = 0; i < tableLength; i++ ) {
            for ( int j = 0; j < tableWidth; j++ ) {
                Log << speedVsPower[j][i] << ", ";
            }
            Log << endl;
        }
    }
    
    unsigned int doInterpolation(unsigned int x, unsigned int xList[], unsigned int yList[] )
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
        
        /*
        Log << "Interpolating to: " << x << endl;
        Log << xLow << ", " << xHigh << ", " << yLow << ", " << yHigh << endl;
        Log << xSpan.getInteger() << ", " << ySpan.getInteger() << endl;
        Log << (double)xRem << ", " << (double)factor << ", " << (double)yRem << endl; 
        Log << y << endl;            
        */

        return y;
    }

    unsigned int GetPowerForFreeSpeed(unsigned int speed) {
        return doInterpolation(speed, speedVsPower[1], speedVsPower[0] );
    }  

    unsigned int GetFreeSpeedForPower(unsigned int power) {
        return doInterpolation(power, speedVsPower[0], speedVsPower[1] );
    }  

    void setPowerAndSpeedPair( int index, int power, int speed )
    {
        speedVsPower[0][index] = power;
        speedVsPower[1][index] = speed;
    }
};

class SpeedToPowerConverterProduction: public SpeedToPowerConverterTest
{
    public:
        SpeedToPowerConverterProduction() 
        {
            this->setEEPROMStartIndex( 0 );
        }
};

#endif