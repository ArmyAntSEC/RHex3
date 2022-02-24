#pragma once

#include <string.h>

struct HardwareEEPROMInterface
{
    virtual void UpdateEEPROM(unsigned address, unsigned value) = 0;
    virtual unsigned int ReadEEPROM(unsigned address) = 0;
};


struct HardwareEEPROMMock: public HardwareEEPROMInterface
{
    static const int EEPROMSize = 256;
    int EEPROMData[EEPROMSize];

    HardwareEEPROMMock()
    {
        resetValues();
    }

    void resetValues()
    {
        memset(EEPROMData, 0, EEPROMSize);
    }

    virtual void UpdateEEPROM(unsigned address, unsigned value)
    {
        if (address < EEPROMSize)
        {
            int valueCapped = value & 0xFF;
            EEPROMData[address] = valueCapped;
        }
    }

    virtual unsigned int ReadEEPROM(unsigned address)
    {
        if (address < EEPROMSize)
        {
            return EEPROMData[address];
        }
        else
        {
            return 0;
        }
    }
};

#ifdef ARDUINO
#include <EEPROM.h>
struct HardwareEEPROM: public HardwareEEPROMInterface
{
    virtual void UpdateEEPROM(unsigned address, unsigned value)
    {
        EEPROM.update(address, value);
    }

    virtual unsigned int ReadEEPROM(unsigned address) 
    {
        return EEPROM.read(address);
    }
};

#endif
