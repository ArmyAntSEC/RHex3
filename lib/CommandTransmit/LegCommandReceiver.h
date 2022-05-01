#pragma once
#include <base.h>
#include <MotorSpeedRegulatorInterfaces.h>
#include <LegCommandBase.h>

class LegCommandReceiver: public I2CBase
{
private:
    MotorSpeedCommanderInterface* commander;

public:
    LegCommandReceiver( MotorSpeedCommanderInterface* _commander ): commander(_commander)
    {}
};

#ifdef ARDUINO

#include <Wire.h>

class I2CTextReceiver: public I2CBase
{    
private:
    I2CTextReceiver() 
    {
        Wire.begin( 7 ); //Hard-coded ID for now. Should be stored in EEPROM
    }
public:
    I2CTextReceiver(I2CTextReceiver const&) = delete; //To make singleton
    void operator=(I2CTextReceiver const&) = delete; //To make singleton.

    static I2CTextReceiver& getSingletonInstance() 
    {
        static I2CTextReceiver instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }    
};

#endif
