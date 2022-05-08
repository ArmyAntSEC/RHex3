#pragma once
#include <base.h>

struct HardwareEEPROMInterface
{
    virtual void UpdateEEPROM(uint8_t address, uint8_t value) = 0;
    virtual uint8_t ReadEEPROM(uint8_t address) = 0;
};

struct HardwareEEPROMMock : public HardwareEEPROMInterface
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

    virtual void UpdateEEPROM(int16_t address, int16_t value)
    {
        if (address < EEPROMSize)
        {
            int16_t valueCapped = value & 0xFF;
            EEPROMData[address] = valueCapped;
        }
    }

    virtual uint16_t ReadEEPROM(int16_t address)
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
struct HardwareEEPROM : public HardwareEEPROMInterface
{
    virtual void UpdateEEPROM(uint8_t address, uint8_t value)
    {
        EEPROM.update(address, value);
    }

    virtual uint8_t ReadEEPROM(uint8_t address)
    {
        return EEPROM.read(address);
    }
};

#endif
