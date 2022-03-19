#pragma once
#include <cstdint>
#include <string.h>

struct HardwareEEPROMInterface
{
    virtual void UpdateEEPROM(unsigned address, unsigned value) = 0;
    virtual uint16_t ReadEEPROM(unsigned address) = 0;
};


struct HardwareEEPROMMock: public HardwareEEPROMInterface
{
    static const int16_t EEPROMSize = 256;
    int16_t EEPROMData[EEPROMSize];

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
            int16_t valueCapped = value & 0xFF;
            EEPROMData[address] = valueCapped;
        }
    }

    virtual uint16_t ReadEEPROM(unsigned address)
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

    virtual unsigned int16_t ReadEEPROM(unsigned address) 
    {
        return EEPROM.read(address);
    }
};

#endif
