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
void storeManuallyMeasuredValuesToEEPROM();

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }
    Serial.println("Hello World!");    

    randomSeed(analogRead(UNCONNECTED_ANALOG));
    pinMode(UNCONNECTED_ANALOG, INPUT);
    
    initOneLeg();
    
    //Register all leg features with the scheduler.    
    recurring10ms.add(&encoderWrapperComputeSpeed);
    recurring10ms.add(&regulator);
    recurring10ms.add(&commander);
    recurring10ms.init(millis());
    sched.add(&recurring10ms);
    
    //storeManuallyMeasuredValuesToEEPROM();
    measurePowerVsSpeedForOneLeg();        
}

void storeManuallyMeasuredValuesToEEPROM()
{
    //First chaeck what we have
    SpeedToPowerConverterTest converter;
    converter.setEEPROMStartIndex(0);
    converter.initFromEEPROM();
    converter.printContent();

    const int tableLength = 7;
    const int tableWidth = 2;
    unsigned int eepromStorageStartIndex = 96; 
    int rawData[tableWidth][tableLength] = {{32, 48, 64, 96, 128, 192, 255}, {485, 1117, 1672, 2409, 2892, 3469, 3903}};
    
    EEPROMStorage eeprom;
    eeprom.writeIntArrayToAddress(eepromStorageStartIndex, (int*)rawData, tableWidth*tableLength ); 
    
    //Then check what we got.        
    converter.initFromEEPROM();
    converter.printContent();
}   

void measurePowerVsSpeedForOneLeg()
{
    const int powerLen = 8;
    int power[powerLen] = {0, 32, 48, 64, 96, 128, 192, 255};

    SpeedToPowerConverterTest converter;
    converter.setEEPROMStartIndex(0);
    Log << "Storing to EEPROM index 0." << endl;

    for (int i = 0; i < powerLen; i++)
    {        
        //Spin up the motor
        driver.setMotorPWM(255);
        sched.delayWithoutStoppingScheduler(500);

        //Set the proper speed and let settle
        driver.setMotorPWM(power[i]);
        sched.delayWithoutStoppingScheduler(1000);

        //Measure the speed
        int speed = encoder->getSpeedCPS(); //Filtered();
        
        converter.setPowerAndSpeedPair(i, power[i], speed);
        
        Log << "Power: " << power[i] << " Speed: " << speed << endl;            
    }
    converter.saveToEEPROM();
    driver.setMotorPWM(0);
    Log << "Values stored to EEPROM" << endl;
}

void loop()
{
    //sched.run();
}
