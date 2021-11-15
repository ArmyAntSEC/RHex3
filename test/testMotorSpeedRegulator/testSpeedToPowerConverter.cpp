#include <unity.h>

#include "MockEEPROMStorage.h"

#define private public
#include <SpeedToPowerConverter.h>

void testCreateConverter()
{
    SpeedToPowerConverter<0> converter;
    

}


void processSpeedToPowerConverter()
{
    RUN_TEST( testCreateConverter );    
}