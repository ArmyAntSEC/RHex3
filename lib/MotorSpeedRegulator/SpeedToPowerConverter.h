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
    
    unsigned int GetPowerForFreeSpeed(unsigned int speed)    
    {        

        //Check if we are out of range
        if (speed <= speedVsPower[1][0])
        {
            return speedVsPower[0][0];
        }
        else if (speed >= speedVsPower[1][6])
        {
            return speedVsPower[0][6];
        }

        int speedIdx = 0;
        while (speedVsPower[1][speedIdx] < speed)
        {
            speedIdx++;
        }

        int speedLow = speedVsPower[1][speedIdx - 1];
        int speedHigh = speedVsPower[1][speedIdx];
        SQ15x16 speedSpan = speedHigh - speedLow;
        int powerLow = speedVsPower[0][speedIdx - 1];
        int powerHigh = speedVsPower[0][speedIdx];
        SQ15x16 powerSpan = powerHigh - powerLow;

        SQ15x16 speedRem = speed - speedLow;
        SQ15x16 factor = powerSpan / speedSpan;
        SQ15x16 powerRem = speedRem * factor;

        int power = roundFixed(powerRem).getInteger() + powerLow;
        /*
        Log << "Interpolating to: " << speed << endl;
        Log << speedLow << ", " << speedHigh << ", " << powerLow << ", " << powerHigh << endl;
        Log << speedSpan.getInteger() << ", " << powerSpan.getInteger() << endl;
        Log << (double)speedRem << ", " << (double)factor << ", " << (double)powerRem << endl; 
        Log << power << endl;            
        */
        return power;
    }

    void setPowerAndSpeedPair( int index, int power, int speed )
    {
        speedVsPower[0][index] = power;
        speedVsPower[1][index] = speed;
    }
};

#endif