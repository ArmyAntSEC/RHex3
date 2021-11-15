#pragma once

#include "EEPROMStorage.h"

template <int startIndex, int N, int M >
class EEPROMBackedArray: public EEPROMStorageInterfacer
{
    private:
        int values[N][M];        
    
    public:
        EEPROMBackedArray()
        {
            #ifndef ARDUINO
            memset(values,0,N*M*sizeof(int));
            #endif
        }

        void setValue( int n, int m, int value )
        {
            values[n][m] = value;
        }

        void loadFromEEPROM()
        {
            eeprom->readIntArrayFromAddress(startIndex,(int*)values,N*M);
        }

        void storeToEEPROM()
        {
            eeprom->writeIntArrayToAddress(startIndex,(int*)values,N*M);
        }

        int const * getSubArray( int index )
        {
            return values[index];
        }

};