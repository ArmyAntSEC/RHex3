#ifndef _EEPROMSTORAGE_H_
#define _EEPROMSTORAGE_H_

#include <HardwareEEPROM.h>
#include <stdint.h>

class EEPROMStorageInterface
{
    public:
    virtual void writeIntToIndex( int16_t index, int16_t value ) = 0;
    virtual int16_t readIntFromIndex( int16_t index ) = 0;
    virtual void writeIntArrayToAddress( int16_t index, int16_t values[], int16_t length ) = 0;
    virtual void readIntArrayFromAddress( int16_t index, int16_t values[], int16_t length ) = 0;
};

class EEPROMStorage: public EEPROMStorageInterface
{
    private:
        HardwareEEPROMInterface* EEPROM;

        virtual int16_t convertIndexToFirstAddress( int16_t index ) {
            return index * 2;
        }
        
        virtual int16_t convertIndexToSecondAddress( int16_t index ) {
            return convertIndexToFirstAddress(index) + 1;
        }
    public:
        EEPROMStorage( HardwareEEPROMInterface* _eeprom): EEPROM(_eeprom)
        {}

        virtual void writeIntToIndex( int16_t index, int16_t value )
        {                                  
            int16_t address1 = convertIndexToFirstAddress(index);
            uint8_t value1 = value & 0xFF;
            EEPROM->UpdateEEPROM( address1, value1 );
            
            int16_t address2 = convertIndexToSecondAddress(index);
            uint8_t value2 = (value >> 8) & 0xFF;
            EEPROM->UpdateEEPROM( address2, value2 );                        
        }

        virtual int16_t readIntFromIndex( int16_t index )
        {            
            int16_t address1 = convertIndexToFirstAddress(index);
            int16_t value1 = EEPROM->ReadEEPROM(address1 );
            
            int16_t address2 = convertIndexToSecondAddress(index);
            int16_t value2 = EEPROM->ReadEEPROM (address2 );

            return value1 + (value2 << 8);         
        }
    
        virtual void writeIntArrayToAddress( int16_t index, int16_t values[], int16_t length )
        {            
            for ( int16_t i = 0; i < length; i++ ) {
                this->writeIntToIndex( index + i, values[i] );
            }         
        }

        virtual void readIntArrayFromAddress( int16_t index, int16_t values[], int16_t length )
        {
            for ( int16_t i = 0; i < length; i++ ) {
                values[i] = this->readIntFromIndex( index + i );
            }
        }
};

#endif