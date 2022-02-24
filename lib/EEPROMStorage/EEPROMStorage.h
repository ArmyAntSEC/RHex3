#ifndef _EEPROMSTORAGE_H_
#define _EEPROMSTORAGE_H_

#include <HardwareEEPROM.h>
#include <stdint.h>

class EEPROMStorageInterface
{
    public:
    virtual void writeIntToIndex( int index, int value ) = 0;
    virtual int readIntFromIndex( int index ) = 0;
    virtual void writeIntArrayToAddress( int index, int values[], int length ) = 0;
    virtual void readIntArrayFromAddress( int index, int values[], int length ) = 0;
};

class EEPROMStorage: public EEPROMStorageInterface
{
    private:
        HardwareEEPROMInterface* EEPROM;

        virtual int convertIndexToFirstAddress( int index ) {
            return index * 2;
        }
        
        virtual int convertIndexToSecondAddress( int index ) {
            return convertIndexToFirstAddress(index) + 1;
        }
    public:
        EEPROMStorage( HardwareEEPROMInterface* _eeprom): EEPROM(_eeprom)
        {}

        virtual void writeIntToIndex( int index, int value )
        {                                  
            int address1 = convertIndexToFirstAddress(index);
            uint8_t value1 = value & 0xFF;
            EEPROM->UpdateEEPROM( address1, value1 );
            
            int address2 = convertIndexToSecondAddress(index);
            uint8_t value2 = (value >> 8) & 0xFF;
            EEPROM->UpdateEEPROM( address2, value2 );                        
        }

        virtual int readIntFromIndex( int index )
        {            
            int address1 = convertIndexToFirstAddress(index);
            int value1 = EEPROM->ReadEEPROM(address1 );
            
            int address2 = convertIndexToSecondAddress(index);
            int value2 = EEPROM->ReadEEPROM (address2 );

            return value1 + (value2 << 8);         
        }
    
        virtual void writeIntArrayToAddress( int index, int values[], int length )
        {            
            for ( int i = 0; i < length; i++ ) {
                this->writeIntToIndex( index + i, values[i] );
            }         
        }

        virtual void readIntArrayFromAddress( int index, int values[], int length )
        {
            for ( int i = 0; i < length; i++ ) {
                values[i] = this->readIntFromIndex( index + i );
            }
        }
};

#endif