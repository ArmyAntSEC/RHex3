#include <base.h>
#ifdef ARDUINO
#include <LegCommandSender.h>

class OnboardBlinkingLed
{
private:
    const int LED_PIN = LED_BUILTIN;
    bool ledOn = false;

public:
    void config()
    {
        pinMode(LED_PIN, OUTPUT);
    }

    void toggleLED()
    {
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn);
    }
} onboardBlinkingLed;

const int32_t clicksPerRev = 1792; // This is not exact, so we will drift about half of a click every round
int32_t slowPos = -250;
int32_t fastPos = 250;
const int32_t halfStepTime = 0.75e6;

LegCommandSender sender;
I2CSenderWrapper i2cWrapper;

int8_t tripodOne[3][2] = {{8, 1}, {9, 0}, {10, 1}};
int8_t tripodTwo[3][2] = {{8, 0}, {9, 1}, {10, 0}};

HardwareClock hwClock;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }
    Log << "*************************" << endl;
    Log << "Hello World" << endl;
    Log << "Master" << endl;

    onboardBlinkingLed.config();
    i2cWrapper.config();
    sender.config(&i2cWrapper);
    sender.configureTripodOne(tripodOne);
    sender.configureTripodOne(tripodTwo);

    Log << "Counting down..." << endl;
    for (int i = 10; i >= 0; i--)
    {
        delay(1000);
        Log << i << "..." << endl;
    }
}

void loop()
{
    Log << "Loop start" << endl;

    sender.sendCommandToTripodOne(slowPos, halfStepTime, &hwClock);
    sender.sendCommandToTripodTwo(fastPos - clicksPerRev, halfStepTime, &hwClock);

    onboardBlinkingLed.toggleLED();
    hwClock.delayMicroseconds(halfStepTime);

    sender.sendCommandToTripodOne(fastPos, halfStepTime, &hwClock);
    sender.sendCommandToTripodTwo(slowPos, halfStepTime, &hwClock);

    onboardBlinkingLed.toggleLED();
    hwClock.delayMicroseconds(halfStepTime);

    fastPos += clicksPerRev;
    slowPos += clicksPerRev;

    Log << "Loop done" << endl;
}

#else
int main()
{
}
#endif

/* Original controller code

#include <Arduino.h>
#include <Wire.h>
#include <SerialStream.h>
int LED = 13;

bool ledOn = false;

const int32_t clicksPerRev = 1792;
int32_t slowPos = -250;
int32_t fastPos = 250;
const int32_t halfStepTime = 0.75e6;
const int controllerID[3] = {8, 9, 10};

void sendCommand(int8_t controllerID, int motorID, int32_t pos, int32_t halfStepTime)
{

    int32_t command[3] = {motorID, pos, halfStepTime};

    Wire.beginTransmission(controllerID);

    for (int i = 0; i < sizeof(command); i++)
    {
        uint8_t *commandBytes = (uint8_t *)(command);
        Wire.write(commandBytes[i]);
    }
    Wire.endTransmission();

    Log << controllerID << ": " << command[0] << " " << command[1] << " " << command[2] << endl;

    int n = Wire.requestFrom(controllerID, 1);
    Log << n;
    while (Wire.available())
    {
        char c = Wire.read();
        Serial.print(c);
    }
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }
    Log << "*************************" << endl;
    Log << "Hello I2C World" << endl;
    Log << "Master" << endl;

    pinMode(13, OUTPUT);

    Wire.begin();

    Log << "Counting down..." << endl;
    for (int i = 10; i >= 0; i--)
    {
        delay(1000);
        Log << i << "..." << endl;
    }
}

void toggleLED()
{
    ledOn = !ledOn;
    digitalWrite(LED, ledOn);
}

void loop()
{
    Log << "Loop start" << endl;

    sendCommand(controllerID[0], 0, slowPos, halfStepTime);
    delay(12);
    sendCommand(controllerID[0], 1, fastPos - clicksPerRev, halfStepTime);
    delay(12);
    sendCommand(controllerID[1], 1, slowPos, halfStepTime);
    delay(12);
    sendCommand(controllerID[1], 0, fastPos - clicksPerRev, halfStepTime);
    delay(12);
    sendCommand(controllerID[2], 0, slowPos, halfStepTime);
    delay(12);
    sendCommand(controllerID[2], 1, fastPos - clicksPerRev, halfStepTime);
    delay(12);

    toggleLED();
    delay(halfStepTime / 1e3);

    sendCommand(controllerID[0], 0, fastPos, halfStepTime);
    delay(12);
    sendCommand(controllerID[0], 1, slowPos, halfStepTime);
    delay(12);
    sendCommand(controllerID[1], 1, fastPos, halfStepTime);
    delay(12);
    sendCommand(controllerID[1], 0, slowPos, halfStepTime);
    delay(12);
    sendCommand(controllerID[2], 0, fastPos, halfStepTime);
    delay(12);
    sendCommand(controllerID[2], 1, slowPos, halfStepTime);
    delay(12);

    toggleLED();
    delay(halfStepTime / 1e3);

    fastPos += clicksPerRev;
    slowPos += clicksPerRev;

    Log << "Loop done" << endl;
}
*/