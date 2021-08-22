#include <Arduino.h>
#include "LevelLogger.h"
#include <configureOneLeg.h>

#include <TaskScheduler.h>
#include <LevelLogger.h>

#include "getFreeMemory.h"
#include "RecurringTaskGroup.h"

#include <configureOneLeg.h>

#include <SpeedToPowerConverter.h>

#define UNCONNECTED_ANALOG A2

TaskScheduler sched;

RecurringTaskGroup<16> recurring10ms(10);

void measurePowerVsSpeedForOneLeg();

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }
    Serial.println("Hello World!");

    DEBUG(F("Setup started"));

    randomSeed(analogRead(UNCONNECTED_ANALOG));
    pinMode(UNCONNECTED_ANALOG, INPUT);

    initOneLeg();

    //Register all leg features with the scheduler.
    recurring10ms.add(&encoderWrapperComputeSpeed);
    recurring10ms.add(&regulator);
    recurring10ms.add(&commander);
    recurring10ms.init(millis());
    sched.add(&recurring10ms);

    measurePowerVsSpeedForOneLeg();
}

void measurePowerVsSpeedForOneLeg()
{
    const int powerLen = 7;
    int power[] = {32, 48, 64, 96, 128, 192, 255};

    SpeedToPowerConverter converter;
    converter.setEEPROMStartIndex(0);

    for (int i = 0; i < powerLen; i++)
    {
        Log << "Power: " << power[i] << " Speed: ";

        long int speedSum = 0;
        int maxSpeed = 0;
        int minSpeed = 32767; //Max of a short int.

        for (int j = 0; j < 5; j++)
        {
            //Spin up the motor
            driver.setMotorPWM(255);
            sched.delayWithScheduler(500);

            //Set the proper speed and let settle
            driver.setMotorPWM(power[i]);
            sched.delayWithScheduler(500);

            //Measure the speed
            int speed = encoder.getSpeedCPS();
            speedSum += speed;
            maxSpeed = max( speed, maxSpeed );
            minSpeed = min( speed, minSpeed );

            Log << speed << ", ";            
        }
        Log << endl;
        Log << "Average speed: " << speedSum / 5 << endl;
        Log << "Speed Span: " << maxSpeed - minSpeed << endl;
        converter.setPowerAndSpeedPair(i, power[i], speedSum / 5);
    }
    converter.saveToEEPROM();
    driver.setMotorPWM(0);
}

void loop()
{
    //sched.run();
}
