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
    unsigned int speedVsPower[tableLength][tableWidth] = {{20, 789}, {24, 1363}, {32, 2145}, {48, 3472}, {64, 4507}, {128, 6509}, {255, 7735}};

    void initFromEEPROM()
    {                        
        int* speedVsPowerAddress = (int*)speedVsPower;
        EEPROMStorage::readIntArrayFromAddress( 0, speedVsPowerAddress, tableLength*tableWidth );        
    }

    void saveToEEPROM()
    {                        
        int* speedVsPowerAddress = (int*)speedVsPower;
        EEPROMStorage::writeIntArrayToAddress( 0, speedVsPowerAddress, tableLength*tableWidth );        
    }

    unsigned int GetPowerForFreeSpeed(unsigned int speed)
    {        

        //Check if we are out of range
        if (speed <= speedVsPower[0][1])
        {
            return speedVsPower[0][0];
        }
        else if (speed >= speedVsPower[6][1])
        {
            return speedVsPower[6][0];
        }

        int speedIdx = 0;
        while (speedVsPower[speedIdx][1] < speed)
        {
            speedIdx++;
        }

        int speedLow = speedVsPower[speedIdx - 1][1];
        int speedHigh = speedVsPower[speedIdx][1];
        SQ15x16 speedSpan = speedHigh - speedLow;
        int powerLow = speedVsPower[speedIdx - 1][0];
        int powerHigh = speedVsPower[speedIdx][0];
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
};

#endif