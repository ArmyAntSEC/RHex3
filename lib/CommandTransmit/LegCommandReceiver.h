#pragma once
#include <base.h>
#include <MotorSpeedRegulatorInterfaces.h>
#include <LegCommandBase.h>

class LegCommandReceiver: public I2CBase
{
private:
    MotorSpeedCommanderInterface* commanderLeft;
    MotorSpeedCommanderInterface* commanderRight;

public:
    LegCommandReceiver( MotorSpeedCommanderInterface* _commanderLeft,  
        MotorSpeedCommanderInterface* _commanderRight  ): 
        commanderLeft(_commanderLeft), commanderRight(_commanderRight)
    {}
};

#ifdef ARDUINO

#include <Wire.h>
#include <HardwareInterrupts.h>

class I2CTextReceiver: public I2CBase
{    
private:
    static const int8_t PacketLength = 4*sizeof(int32_t);
    static volatile int32_t PacketBuffer[PacketLength];
    static volatile bool PacketArrived;

    I2CTextReceiver() 
    {
        Wire.begin( 7 ); //Hard-coded ID for now. Should be stored in EEPROM
        PacketArrived = false;
    }
    
    LegCommandReceiver* receiver;

public:
    I2CTextReceiver(I2CTextReceiver const&) = delete; //To make singleton
    void operator=(I2CTextReceiver const&) = delete; //To make singleton.

    static I2CTextReceiver& getSingletonInstance() 
    {
        static I2CTextReceiver instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }        

    static void receivePacket( int8_t lengthInBytes )
    {        
        if ( lengthInBytes == PacketLength )
        {
            for ( int8_t i = 0; i < PacketLength; i++ ) {
                PacketBuffer[i] = Wire.read();
            }
            PacketArrived = true;
        }
    }


};

#endif
