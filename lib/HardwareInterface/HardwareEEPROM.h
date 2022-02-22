#pragma once

#ifdef ARDUINO
struct HardwareEEPROM
{
    void UpdateEEPROM(unsigned address, unsigned value)
    {
        EEPROM.update(address, value);
    }

    unsigned int ReadEEPROM(unsigned address) return EEPROM.read(address);
}
}
;
#else
#include <string.h>
struct HardwareEEPROM
{
    static const int EEPROMSize = 256;
    int EEPROMData[EEPROMSize];

    HardwareEEPROM()
    {
        resetValues();
    }

    void resetValues()
    {
        memset(EEPROMData, 0, EEPROMSize);
    }

    void UpdateEEPROM(unsigned address, unsigned value)
    {
        if (address < EEPROMSize)
        {
            int valueCapped = value & 0xFF;
            EEPROMData[address] = valueCapped;
        }
    }

    unsigned int ReadEEPROM(unsigned address)
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

#endif