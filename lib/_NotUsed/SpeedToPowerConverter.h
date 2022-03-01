#ifndef _SPEEDTOPOWERCONVERTER_H_
#define _SPEEDTOPOWERCONVERTER_H_

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

#endif