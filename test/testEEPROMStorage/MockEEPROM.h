#pragma once
#include <EEPROMStorage.h>

class MockEEPROMStorage: public EEPROMStorageInterface
{
    
    public:
    int* rawData;
    
    virtual void writeIntToIndex( int index, int value ) {}
    
    virtual int readIntFromIndex( int index ) { return -1; }
    
    virtual void writeIntArrayToAddress( int index, int values[], int length ) 
    {
        memcpy(rawData+index,values,length*sizeof(int));
    }
    
    virtual void readIntArrayFromAddress( int index, int values[], int length )
    {
        memcpy(values,rawData+index,length*sizeof(int));
    }

};