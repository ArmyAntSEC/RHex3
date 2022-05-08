#include <unityEx.h>
#define private public
#include <LegCommandSender.h>
#include <RunnableInterface.h>

struct I2CSenderWrapperMock : public I2CSenderWrapperInterface
{
    int8_t lastAddress[3];
    int32_t lastCommand[3][3];
    int8_t lastBufferLength[3];
    int8_t writePointer = 0;
    bool returnValue = true;

    virtual bool send(uint8_t address, uint8_t *buffer, int8_t bufferLength)
    {
        lastAddress[writePointer] = address;
        lastBufferLength[writePointer] = bufferLength;
        memcpy((uint8_t *)lastCommand[writePointer], buffer, bufferLength);

        writePointer++;

        return returnValue;
    }
};

struct TaskAwareDelayMock : public TaskAwareDelayInterface
{
    int32_t lastDelay[3];
    int8_t writePointer = 0;

    virtual void delayMicros(int32_t micros)
    {
        lastDelay[writePointer] = micros;
        writePointer++;
    }
};

void testConfigSender()
{
    LegCommandSender sender;
    I2CSenderWrapperMock i2cMock;
    sender.config(&i2cMock);

    TEST_ASSERT_EQUAL(&i2cMock, sender.i2cWrapper);
}

void testSendCommand()
{
    LegCommandSender sender;
    I2CSenderWrapperMock i2cMock;
    sender.config(&i2cMock);

    int8_t driverID = 12;
    int8_t motorID = 1;
    int32_t position = 4567;
    int32_t relativeTime = 1.5e6;

    bool rValue = sender.sendCommand(driverID, motorID, position, relativeTime);

    TEST_ASSERT_EQUAL(driverID, i2cMock.lastAddress[0]);
    TEST_ASSERT_EQUAL(motorID, i2cMock.lastCommand[0][0]);
    TEST_ASSERT_EQUAL(position, i2cMock.lastCommand[0][1]);
    TEST_ASSERT_EQUAL(relativeTime, i2cMock.lastCommand[0][2]);
    TEST_ASSERT_TRUE(rValue);
}

void testConfigureTripod()
{
    LegCommandSender sender;
    I2CSenderWrapperMock i2cMock;
    sender.config(&i2cMock);

    int8_t tripodOne[3][2] = {{8, 1}, {9, 0}, {10, 1}};

    sender.configureTripodOne(tripodOne);

    TEST_ASSERT_EQUAL_INT8_ARRAY(tripodOne[0], sender.tripodOne[0], 2);
    TEST_ASSERT_EQUAL_INT8_ARRAY(tripodOne[1], sender.tripodOne[1], 2);
    TEST_ASSERT_EQUAL_INT8_ARRAY(tripodOne[2], sender.tripodOne[2], 2);

    int8_t tripodTwo[3][2] = {{8, 1}, {9, 0}, {10, 1}};
    sender.configureTripodTwo(tripodTwo);

    TEST_ASSERT_EQUAL_INT8_ARRAY(tripodTwo[0], sender.tripodTwo[0], 2);
    TEST_ASSERT_EQUAL_INT8_ARRAY(tripodTwo[1], sender.tripodTwo[1], 2);
    TEST_ASSERT_EQUAL_INT8_ARRAY(tripodTwo[2], sender.tripodTwo[2], 2);
}

void sendCommandToTripodOne()
{
    LegCommandSender sender;
    I2CSenderWrapperMock i2cMock;
    sender.config(&i2cMock);
    int8_t tripodOne[3][2] = {{8, 1}, {9, 0}, {10, 1}};
    sender.configureTripodOne(tripodOne);

    TaskAwareDelayMock delayMock;

    int32_t position = 4567;
    int32_t relativeTime = 1.5e6;

    sender.sendCommandToTripodOne(position, relativeTime, &delayMock);

    TEST_ASSERT_EQUAL(8, i2cMock.lastAddress[0]);
    TEST_ASSERT_EQUAL(9, i2cMock.lastAddress[1]);
    TEST_ASSERT_EQUAL(10, i2cMock.lastAddress[2]);

    TEST_ASSERT_EQUAL(1, i2cMock.lastCommand[0][0]);
    TEST_ASSERT_EQUAL(0, i2cMock.lastCommand[1][0]);
    TEST_ASSERT_EQUAL(1, i2cMock.lastCommand[2][0]);

    TEST_ASSERT_EQUAL(position, i2cMock.lastCommand[0][1]);
    TEST_ASSERT_EQUAL(position, i2cMock.lastCommand[1][1]);
    TEST_ASSERT_EQUAL(position, i2cMock.lastCommand[2][1]);

    TEST_ASSERT_EQUAL(relativeTime, i2cMock.lastCommand[0][2]);
    TEST_ASSERT_EQUAL(relativeTime, i2cMock.lastCommand[1][2]);
    TEST_ASSERT_EQUAL(relativeTime, i2cMock.lastCommand[2][2]);

    TEST_ASSERT_EACH_EQUAL_INT32(12e3, delayMock.lastDelay, 3);
}

void sendCommandToTripodTwo()
{
    LegCommandSender sender;
    I2CSenderWrapperMock i2cMock;
    sender.config(&i2cMock);
    int8_t tripodTwo[3][2] = {{8, 0}, {9, 1}, {10, 0}};
    sender.configureTripodTwo(tripodTwo);

    int32_t position = 4567;
    int32_t relativeTime = 1.5e6;

    TaskAwareDelayMock delayMock;

    sender.sendCommandToTripodTwo(position, relativeTime, &delayMock);

    TEST_ASSERT_EQUAL(8, i2cMock.lastAddress[0]);
    TEST_ASSERT_EQUAL(9, i2cMock.lastAddress[1]);
    TEST_ASSERT_EQUAL(10, i2cMock.lastAddress[2]);

    TEST_ASSERT_EQUAL(0, i2cMock.lastCommand[0][0]);
    TEST_ASSERT_EQUAL(1, i2cMock.lastCommand[1][0]);
    TEST_ASSERT_EQUAL(0, i2cMock.lastCommand[2][0]);

    TEST_ASSERT_EQUAL(position, i2cMock.lastCommand[0][1]);
    TEST_ASSERT_EQUAL(position, i2cMock.lastCommand[1][1]);
    TEST_ASSERT_EQUAL(position, i2cMock.lastCommand[2][1]);

    TEST_ASSERT_EQUAL(relativeTime, i2cMock.lastCommand[0][2]);
    TEST_ASSERT_EQUAL(relativeTime, i2cMock.lastCommand[1][2]);
    TEST_ASSERT_EQUAL(relativeTime, i2cMock.lastCommand[2][2]);

    TEST_ASSERT_EACH_EQUAL_INT32(12e3, delayMock.lastDelay, 3);
}

void runAllTestsLegCommandSender()
{
    UNITY_BEGIN_INT();
    RUN_TEST(testConfigSender);
    RUN_TEST(testSendCommand);
    RUN_TEST(testConfigureTripod);
    RUN_TEST(sendCommandToTripodOne);
    RUN_TEST(sendCommandToTripodTwo);
    UNITY_END_INT();
}