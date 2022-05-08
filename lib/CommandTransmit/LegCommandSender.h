#pragma once
#include <base.h>
#include "LegCommandBase.h"
#include <RunnableInterface.h>

struct I2CSenderWrapperInterface
{
    virtual bool send(uint8_t address, uint8_t *buffer, int8_t bufferLength) = 0;
};

class LegCommandSender : public I2CBase
{
private:
    I2CSenderWrapperInterface *i2cWrapper;
    int8_t tripodOne[3][2];
    int8_t tripodTwo[3][2];

    void sendCommandToTripod(int8_t tripod[3][2], int32_t position, int32_t relativeTimeMicros, TaskAwareDelayInterface *delay)
    {
        for (int i = 0; i < 3; i++)
        {
            int8_t driverID = tripod[i][0];
            int8_t motorID = tripod[i][1];
            sendCommand(driverID, motorID, position, relativeTimeMicros);
            delay->delayMicros(12e3);
        }
    }

public:
    void config(I2CSenderWrapperInterface *_i2cWrapper)
    {
        i2cWrapper = _i2cWrapper;
    }

    bool sendCommand(int8_t driverID, int8_t motorID, int32_t position, int32_t relativeTimeMicros)
    {
        int32_t buffer[3] = {motorID, position, relativeTimeMicros};
        return i2cWrapper->send(driverID, (uint8_t *)buffer, sizeof(buffer));
    }

    void configureTripodOne(int8_t _tripodOne[3][2])
    {
        memcpy(tripodOne, _tripodOne, sizeof(tripodOne));
    }

    void configureTripodTwo(int8_t _tripodTwo[3][2])
    {
        memcpy(tripodTwo, _tripodTwo, sizeof(tripodTwo));
    }

    void sendCommandToTripodOne(int32_t position, int32_t relativeTimeMicros, TaskAwareDelayInterface *delay)
    {
        sendCommandToTripod(tripodOne, position, relativeTimeMicros, delay);
    }

    void sendCommandToTripodTwo(int32_t position, int32_t relativeTimeMicros, TaskAwareDelayInterface *delay)
    {
        sendCommandToTripod(tripodTwo, position, relativeTimeMicros, delay);
    }
};

#ifdef ARDUINO
#include <Wire.h>
class I2CSenderWrapper : public I2CSenderWrapperInterface
{
public:
    void config()
    {
        Wire.begin();
    }

    virtual bool send(uint8_t address, uint8_t *buffer, int8_t bufferLength)
    {
        Wire.beginTransmission(address);
        for (int i = 0; i < bufferLength; i++)
        {
            Wire.write(buffer[i]);
        }
        Wire.endTransmission();

        Wire.requestFrom(address, 2);
        char *expectedResponse = "OK";
        bool responseOK = true;
        responseOK &= (Wire.read() == expectedResponse[0]);
        responseOK &= (Wire.read() == expectedResponse[1]);
        return responseOK;
    }
};

#endif