#pragma once
#include <base.h>
#include <MotorSpeedRegulatorInterfaces.h>
#include <ArduinoJson.h>

struct I2cTextSenderInterface
{
    virtual void sendText( int8_t i2cAddress, char * str, int8_t len ) = 0;
};

enum LegIdentifiers { FORWARD_LEFT = 0, FORWARD_RIGHT = 1, 
    MIDDLE_LEFT = 2, MIDDLE_RIGHT = 3, 
    BACK_LEFT = 4, BACK_RIGHT = 5 };

struct I2CBase 
{
    static const int LegAddresses[6][2];

    int8_t getControllerIDForLeg(LegIdentifiers leg )
    {
        return LegAddresses[leg][0];
    }

    int8_t getMotorIDForLeg(LegIdentifiers leg )
    {
        return LegAddresses[leg][1];
    }
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
        const int8_t bufferLength = 120;
        char buffer[bufferLength];

        StaticJsonDocument<200> doc;        
        doc["motorID"] = getMotorIDForLeg( leg );
        doc["targetPositionClicks"] = goal.targetPositionClicks;
        doc["targetRelativeTimeMicros"] = goal.targetRelativeTimeMicros;
        
        serializeJson( doc, buffer, bufferLength );
        
        int8_t controllerID = getControllerIDForLeg( leg );
        rawSender->sendText( controllerID, buffer, bufferLength );
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

    virtual void sendText( int8_t i2cAddress, char * str, int8_t len )
    {
        Wire.beginTransmission(i2cAddress);
        Wire.write(str, len);
        int8_t errorCode = Wire.endTransmission();
        if ( errorCode ) {
            Log << "I2C Error: " << errorCode << endl;
        }
    }            
};

#endif
