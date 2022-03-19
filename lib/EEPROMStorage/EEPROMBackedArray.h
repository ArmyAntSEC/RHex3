#pragma once

#include "EEPROMStorage.h"
template <int16_t N, int16_t M >
class EEPROMBackedArrayInterface
{
    public:        
    virtual int16_t const * getSubArray( int16_t n ) = 0;
    int16_t getArrayLength() { return M; }
};

template <int16_t N, int16_t M >
class EEPROMBackedArray: public EEPROMBackedArrayInterface<N,M>
{
    private:
        EEPROMStorageInterface* eeprom;
        int16_t values[N][M];   
        const int16_t startIndex;     
    
    public:
        EEPROMBackedArray( EEPROMStorageInterface* _eeprom, int16_t _startIndex ):
            startIndex(_startIndex)
        {
            eeprom = _eeprom;            
            memset(values,0,N*M*sizeof(int));
        }

        void setValue( int16_t n, int16_t m, int16_t value )
        {
            if ( n < N && m < M )
                values[n][m] = value;

        }

        void loadFromEEPROM()
        {
            eeprom->readIntArrayFromAddress(startIndex,(int16_t*)values,N*M);
        }

        void storeToEEPROM()
        {
            eeprom->writeIntArrayToAddress(startIndex,(int16_t*)values,N*M);
        }
        
        virtual int16_t const * getSubArray( int16_t n )
        {            
            return values[n];            
        }
};