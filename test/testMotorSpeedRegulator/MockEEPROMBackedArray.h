#pragma once
#include <EEPROMBackedArray.h>


template <int16_t N, int16_t M >
class MockEEPROMBackedArray: public EEPROMBackedArrayInterface<N,M>
{
    public:    
    int16_t counter;
    int16_t data[2][8] = {{1, 3, 5, 7, 9, 11, 13, 15}, {3, 5, 7, 9, 11, 13, 15, 17}};
    
    virtual int16_t const * getSubArray( int16_t n )
    {        
        static_assert( N == 2 && M == 8, "N and M must be 2 and 3 respectively" );
        return data[n];
    }
};
