#pragma once
#include <EEPROMBackedArray.h>

template <int N, int M >
class MockEEPROMBackedArray: public EEPROMBackedArrayInterface<N,M>
{
    public:    
    int counter;
    
    virtual int const * getSubArray( int n )
    {        
        return 0;
    }
};
