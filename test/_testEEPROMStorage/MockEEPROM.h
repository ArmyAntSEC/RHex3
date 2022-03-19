#pragma once
#include <EEPROMStorage.h>

class MockEEPROMStorage: public EEPROMStorageInterface
{
    
    public:
    int* rawData;
    
    virtual void writeIntToIndex( int16_t index, int16_t value ) {}
    
    virtual int16_t readIntFromIndex( int16_t index ) { return -1; }
    
    virtual void writeIntArrayToAddress( int16_t index, int16_t values[], int16_t length ) 
    {
        memcpy(rawData+index,values,length*sizeof(int));
    }
    
    virtual void readIntArrayFromAddress( int16_t index, int16_t values[], int16_t length )
    {
        memcpy(values,rawData+index,length*sizeof(int));
    }

};