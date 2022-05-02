#pragma once
#include <base.h>
#include <MotorSpeedRegulatorInterfaces.h>
#include <LegCommandBase.h>
#include <HardwareInterrupts.h>
#include <RunnableInterface.h>
#include <HardwareClock.h>

struct I2CReceiverWrapperInterface
{    
    virtual void setOnReceive( void(*onReceive)(int32_t numBytes) ) = 0;
    virtual uint8_t read() = 0;
};

class LegCommandReceiver: public I2CBase
{    
private:    
    static const int8_t messageBufferLength = 3;
    int32_t messageBuffer[messageBufferLength];

    I2CReceiverWrapperInterface* i2cPort;
    
    MotorSpeedCommanderInterface* commanderLeft;
    MotorSpeedCommanderInterface* commanderRight;

    HardwareClockInterface* hwClock;

    static void onReceive( int32_t numberOfBytes )
    {
        LegCommandReceiver& receiver = LegCommandReceiver::getSingletonInstance();        
        receiver.handleIncomingData( numberOfBytes );
    }

    void handleIncomingData( int32_t numberOfBytes )
    {                
        if ( numberOfBytes == 3*sizeof(int32_t) ) {
            int32_t nowMicros = hwClock->getMicrosecondsSinceBoot();
            readMessageToBuffer ( messageBuffer, numberOfBytes );            
            parseMessageAndSendToSpeedCommander( messageBuffer, nowMicros );            
        } else {
            memset( messageBuffer, 0, sizeof(messageBuffer) );
        }
    }

    void parseMessageAndSendToSpeedCommander( int32_t messageBuffer[3], int32_t nowMicros )
    {                
        MotorCommanderGoal goal( messageBuffer[1], messageBuffer[2] );
        int32_t motorID = messageBuffer[0];
        if ( motorID == 0 ) {
            commanderLeft->setGoal( goal, nowMicros );
        } else if ( motorID == 1 ) {
            commanderRight->setGoal( goal, nowMicros );    
        }

    }

    void readMessageToBuffer( int32_t* messageBuffer, int8_t numberOfBytes )
    {
        uint8_t* messageByteBuffer = (uint8_t*)messageBuffer;
        for ( int i = 0; i < numberOfBytes; i++ ) {
            uint8_t byte = i2cPort->read();
            messageByteBuffer[i] = byte;                        
        }                            
    }

    LegCommandReceiver()  //Here to ensure it becomes private.
    {}
        

public:
    LegCommandReceiver(LegCommandReceiver const&) = delete; //To make singleton
    void operator=(LegCommandReceiver const&) = delete; //To make singleton.

    static LegCommandReceiver& getSingletonInstance() 
    {
        static LegCommandReceiver instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }        

    void config( 
        I2CReceiverWrapperInterface* _i2cPort,
        MotorSpeedCommanderInterface* _commanderLeft,
        MotorSpeedCommanderInterface* _commanderRight,
        HardwareClockInterface* _clock )
    {
        i2cPort = _i2cPort;
        i2cPort->setOnReceive( LegCommandReceiver::onReceive );

        commanderLeft = _commanderLeft;
        commanderRight = _commanderRight;

        hwClock = _clock;
    }            
};

#ifdef ARDUINO

#include <Wire.h>

class I2CReceiverWrapper: public I2CReceiverWrapperInterface
{
    I2CReceiverWrapper( int8_t address )
    {
        Wire.begin( address ); //Should only be called once.
    }

    void setOnReceive( void(*_onReceive)(int32_t numBytes) )
    {
        Wire.onReceive(_onReceive);
    }

    uint8_t read()
    {
        return Wire.read();
    }
};

#endif
