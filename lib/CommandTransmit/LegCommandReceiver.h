#pragma once
#include <base.h>
#include <MotorSpeedRegulatorInterfaces.h>
#include <LegCommandBase.h>
#include <HardwareInterrupts.h>
#include <RunnableInterface.h>

struct I2CReceiverWrapperInterface
{    
    virtual void setOnReceive( void(*onReceive)(int32_t numBytes) ) = 0;
    virtual uint8_t read() = 0;
};

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


class I2CReceiver: public I2CBase
{    
private:
    
    static int32_t messageBuffer[4];

    static I2CReceiverWrapperInterface* i2cPort;
    
    static void onReceive( int numberOfBytes )
    {
        uint8_t* messageByteBuffer = (uint8_t*)messageBuffer;
        
        if ( numberOfBytes == 4*sizeof(int32_t) ) {
            for ( int i = 0; i < sizeof(messageBuffer); i++ ) {
                uint8_t byte = i2cPort->read();
                messageByteBuffer[i] = byte;            
            }        

        } else {
            memset( messageBuffer, 0, sizeof(messageBuffer) );
        }
    }

    I2CReceiver() 
    {        
    }
        

public:
    I2CReceiver(I2CReceiver const&) = delete; //To make singleton
    void operator=(I2CReceiver const&) = delete; //To make singleton.

    static I2CReceiver& getSingletonInstance() 
    {
        static I2CReceiver instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }        

    static void configI2CPort( I2CReceiverWrapperInterface* _i2cPort )
    {
        I2CReceiver::i2cPort = _i2cPort;
        i2cPort->setOnReceive( I2CReceiver::onReceive );
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
