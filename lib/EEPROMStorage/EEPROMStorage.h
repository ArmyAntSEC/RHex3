#ifndef _EEPROMSTORAGE_H_
#define _EEPROMSTORAGE_H_

#ifdef ARDUINO
#include <EEPROM.h>
#endif

class EEPROMStorage
{
    public:
        static int convertIndexToFirstAddress( int index ) {
            return index * 2;
        }
        
        static int convertIndexToSecondAddress( int index ) {
            return convertIndexToFirstAddress(index) + 1;
        }

        static void writeIntToIndex( int index, int value )
        {                      
            #ifdef ARDUINO
            int address1 = convertIndexToFirstAddress(index);
            uint8_t value1 = value & 0xFF;
            EEPROM.update(address1, value1 );
            
            int address2 = convertIndexToSecondAddress(index);
            uint8_t value2 = (value >> 8) & 0xFF;
            EEPROM.update(address2, value2 );            
            #endif
        }

        static int readIntFromIndex( int index )
        {
            #ifdef ARDUINO
            int address1 = convertIndexToFirstAddress(index);
            int value1 = EEPROM.read(address1 );
            
            int address2 = convertIndexToSecondAddress(index);
            int value2 = EEPROM.read (address2 );

            return value1 + (value2 << 8);
            #else
            return 0;
            #endif
        }

        static void writeIntArrayToAddress( int index, int values[], int length )
        {            
            for ( int i = 0; i < length; i++ ) {
                EEPROMStorage::writeIntToIndex( index + i, values[i] );
            }         
        }

        static void readIntArrayFromAddress( int index, int values[], int length )
        {
            for ( int i = 0; i < length; i++ ) {
                values[i] = EEPROMStorage::readIntFromIndex( index + i );
            }
        }
};

#endif