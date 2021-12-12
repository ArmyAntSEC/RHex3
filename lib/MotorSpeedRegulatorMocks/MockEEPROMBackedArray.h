#pragma once
#include <EEPROMBackedArray.h>


template <int N, int M >
class MockEEPROMBackedArray: public EEPROMBackedArrayInterface<N,M>
{
    public:    
    int counter;
    int data[2][8] = {{1, 3, 5, 7, 9, 11, 13, 15}, {3, 5, 7, 9, 11, 13, 15, 17}};
    
    virtual int const * getSubArray( int n )
    {        
        static_assert( N == 2 && M == 8, "N and M must be 2 and 3 respectively" );
        return data[n];
    }
};
