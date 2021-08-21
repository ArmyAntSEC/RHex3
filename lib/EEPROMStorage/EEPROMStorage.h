#ifndef _EEPROMSTORAGE_H_
#define _EEPROMSTORAGE_H_

#include <EEPROM.h>

class EEPROMStorage
{
    public:
        static void writeIntToAddress( int address, int value )
        {
            EEPROM.update(address, value);
        }

        static int readIntFromAddress( int address )
        {
            return EEPROM.read(address);
        }

        static void writeIntArrayToAddress( int address, int values[], int length )
        {
            for ( int i = 0; i < length; i++ ) {
                EEPROM.update(address + i, values[i] );
            }
        }

        static void readIntArrayFromAddress( int address, int values[], int length )
        {
            for ( int i = 0; i < length; i++ ) {
                values[i] = EEPROM.read( address + i );
            }
        }
};

#endif