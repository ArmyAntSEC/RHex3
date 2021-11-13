#ifndef _EEPROMSTORAGE_H_
#define _EEPROMSTORAGE_H_

#include <HardwareInterface.h>

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
            int address1 = convertIndexToFirstAddress(index);
            uint8_t value1 = value & 0xFF;
            HardwareInterface::UpdateEEPROM( address1, value1 );
            
            int address2 = convertIndexToSecondAddress(index);
            uint8_t value2 = (value >> 8) & 0xFF;
            HardwareInterface::UpdateEEPROM( address2, value2 );                        
        }

        static int readIntFromIndex( int index )
        {            
            int address1 = convertIndexToFirstAddress(index);
            int value1 = HardwareInterface::ReadEEPROM(address1 );
            
            int address2 = convertIndexToSecondAddress(index);
            int value2 = HardwareInterface::ReadEEPROM (address2 );

            return value1 + (value2 << 8);         
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