#pragma once
#include <base.h>
#include <MotorSpeedRegulatorInterfaces.h>
#include <LegCommandBase.h>

struct I2cTextSenderInterface
{
    virtual void sendText( int8_t i2cAddress, int32_t const * buffer, int8_t len ) = 0;
};

class LegCommandSender: public I2CBase
{
private:
    I2cTextSenderInterface* rawSender;

public:
    LegCommandSender( I2cTextSenderInterface* _rawSender ): rawSender(_rawSender)
    {}

    void sendCommand ( LegIdentifiers leg, MotorCommanderGoal goal )
    {
        int8_t controllerID = getControllerIDForLeg( leg );
        int8_t motorID = getMotorIDForLeg( leg );

        const int32_t buffer[] = 
            {motorID, goal.targetPositionClicks, goal.targetRelativeTimeMicros };            
        
        rawSender->sendText( controllerID, buffer, 3 );
    }    
};

#ifdef ARDUINO

#include <Wire.h>

class I2CTextSender: public I2CBase, public I2cTextSenderInterface
{    
private:
    I2CTextSender() 
    {
        Wire.begin();
    }
public:
    I2CTextSender(I2CTextSender const&) = delete; //To make singleton
    void operator=(I2CTextSender const&) = delete; //To make singleton.

    static I2CTextSender& getSingletonInstance() 
    {
        static I2CTextSender instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }

    virtual void sendText( int8_t i2cAddress, int32_t const * buffer, int8_t len )
    {
        Wire.beginTransmission(i2cAddress);
        Wire.write((uint8_t*)buffer, len*sizeof(buffer));
        int8_t errorCode = Wire.endTransmission();
        if ( errorCode ) {
            Log << "I2C Error: " << errorCode << endl;
        }
    }            
};

#endif
