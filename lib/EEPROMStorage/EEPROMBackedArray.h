#pragma once

#include "EEPROMStorage.h"
template <int N, int M >
class EEPROMBackedArrayInterface
{
    public:        
    virtual int const * getSubArray( int n ) = 0;
    int getArrayLength() { return M; }
};

template <int N, int M >
class EEPROMBackedArray: public EEPROMBackedArrayInterface<N,M>
{
    private:
        EEPROMStorageInterface* eeprom;
        int values[N][M];   
        const int startIndex;     
    
    public:
        EEPROMBackedArray( EEPROMStorageInterface* _eeprom, int _startIndex ):
            startIndex(_startIndex)
        {
            eeprom = _eeprom;

            #ifndef ARDUINO
            memset(values,0,N*M*sizeof(int));
            #endif
        }

        void setValue( int n, int m, int value )
        {
            if ( n < N && m < M )
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
        
        virtual int const * getSubArray( int n )
        {            
            if ( n < N )
                return values[n];
            else
                return 0;            
        }
};