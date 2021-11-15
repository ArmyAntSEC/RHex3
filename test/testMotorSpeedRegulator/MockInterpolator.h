#pragma once
#include <Interpolator.h>

class MockInterpolator: public InterpolatorInterface
{
    public:
    unsigned argumentLog[256];
    unsigned argumentPointer = 0;

    virtual unsigned doInterpolation(unsigned x, unsigned xList[], unsigned yList[], unsigned tableLength )
    {
        argumentLog[argumentPointer++] = x;
        argumentLog[argumentPointer++] = (int)reinterpret_cast<std::uintptr_t>(xList);
        argumentLog[argumentPointer++] = (int)reinterpret_cast<std::uintptr_t>(yList);
        argumentLog[argumentPointer++] = tableLength;
        return argumentPointer;
    }
};
