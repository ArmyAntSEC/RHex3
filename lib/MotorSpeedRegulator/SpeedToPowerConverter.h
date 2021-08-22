#ifndef _SPEEDTOPOWERCONVERTER_H_
#define _SPEEDTOPOWERCONVERTER_H_

#include <FixedPointsCommon.h>
#include <EEPROMStorage.h>
#include <LevelLogger.h>

/* Measured values on the test motor
Power: 32 Average speed: 485 Speed Span: 194
Power: 48 Average speed: 1117 Speed Span: 222
Power: 64 Average speed: 1672 Speed Span: 192
Power: 96 Average speed: 2409 Speed Span: 242
Power: 128 Average speed: 2892 Speed Span: 111
Power: 192 Average speed: 3469 Speed Span: 116
Power: 255 Average speed: 3903 Speed Span: 26

Matlab format:
[32 485 194;
48 1117 222;
64 1672 192;
96 2409 242;
128 2892 111;
192 3469 116;
255 3903 26]
*/

class SpeedToPowerConverterTest
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