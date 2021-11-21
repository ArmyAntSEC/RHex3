#ifndef _SPEEDTOPOWERCONVERTER_H_
#define _SPEEDTOPOWERCONVERTER_H_

#include <FixedPointsCommon.h>
#include <EEPROMStorage.h>
#include <SerialStream.h>
#include "Interpolator.h"
#include <EEPROMBackedArray.h>

class SpeedToPowerConverter
{
    private:
        EEPROMBackedArrayInterface<2,8>* data;   
        Interpolator interpolator;     

    public:
        SpeedToPowerConverter( EEPROMBackedArrayInterface<2,8>* _data ):
            data(_data)
        {}

        unsigned int GetPowerForFreeSpeed(unsigned int speed) {
            int const * speedData = data->getSubArray(0);
            int const * powerData = data->getSubArray(1);            
            int len = data->getArrayLength();
            
            return interpolator.doInterpolation(speed, speedData, 
                powerData, len );                        
        }  

        unsigned int GetFreeSpeedForPower(unsigned int power) {
            int const * speedData = data->getSubArray(0);
            int const * powerData = data->getSubArray(1);            
            int len = data->getArrayLength();
            
            return interpolator.doInterpolation(power, powerData, 
                speedData, len );                        

        }  


};

/*
class SpeedToPowerConverterTest
{

public:
    static const int tableLength = 8;
    static const int tableWidth = 2;

    Interpolator interpolator;
    EEPROMStorage eeprom;

    
    //We default to the end of the EEPROM which we use for testing to not accidentally
    //overwrite the measured values at the start of the EEPROM array.
    unsigned int eepromStorageStartIndex = 96; 
    unsigned int speedVsPower[tableWidth][tableLength] = {{0, 20, 24, 32, 48, 64, 128, 255}, {0, 789, 1363, 2145, 3472, 4507, 6509, 7735}};

    SpeedToPowerConverterTest()
    {        
    }

    void setEEPROMStartIndex( int startIndex )
    {
        this->eepromStorageStartIndex = startIndex;
    }

    void initFromEEPROM()
    {                        
        int* speedVsPowerAddress = (int*)speedVsPower;
        eeprom.readIntArrayFromAddress( eepromStorageStartIndex, speedVsPowerAddress, tableLength*tableWidth );        
    }

    void saveToEEPROM()
    {                        
        int* speedVsPowerAddress = (int*)speedVsPower;
        eeprom.writeIntArrayToAddress( eepromStorageStartIndex, speedVsPowerAddress, tableLength*tableWidth );        
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

    unsigned int GetPowerForFreeSpeed(unsigned int speed) {
        return 0; //interpolator->doInterpolation(speed, speedVsPower[1], 
            //speedVsPower[0], tableLength );
    }  

    unsigned int GetFreeSpeedForPower(unsigned int power) {
        return 0; //interpolator->doInterpolation(power, speedVsPower[0], 
           // speedVsPower[1], tableLength );
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
*/
#endif