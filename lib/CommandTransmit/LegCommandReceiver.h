#pragma once
#include <base.h>
#include <MotorSpeedRegulatorInterfaces.h>
#include <LegCommandBase.h>
#include <HardwareInterrupts.h>
#include <RunnableInterface.h>

struct I2CReceiverWrapperInterface
{    
    virtual void setOnReceive( void(onReceive)(int32_t numBytes) ) = 0;
    virtual int8_t read() = 0;
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


class I2CTextReceiver: public I2CBase
{    
private:
    
    I2CTextReceiver() 
    {        
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
    
};

#ifdef ARDUINO

#include <Wire.h>

class I2CReceiverWrapper: public I2CReceiverWrapperInterface
{
    I2CReceiverWrapper( int8_t address )
    {
        Wire.begin( address ); //Should only be called once.
    }

    void setOnReceive( void(onReceive)(int32_t numBytes) )
    {
        Wire.onReceive(onReceive);
    }

    int8_t read()
    {
        return Wire.read();
    }
};

#endif
