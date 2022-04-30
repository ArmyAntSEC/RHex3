#pragma once

#include <MotorSpeedRegulatorInterfaces.h>
#include <ArduinoJson.h>

class MotorCommanderGoalSerializer
{
    public:
    void serialize( MotorCommanderGoal goal, char* buffer, int8_t bufferLength )
    {
        StaticJsonDocument<200> doc;
        doc["targetPositionClicks"] = goal.targetPositionClicks;
        doc["targetRelativeTimeMicros"] = goal.targetRelativeTimeMicros;

        serializeJson( doc, buffer, bufferLength );
    }

    MotorCommanderGoal deserialize( char* buffer )
    {
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, buffer);
        
        if ( error ) {
            return MotorCommanderGoal();
        } else {
            return MotorCommanderGoal( doc["targetPositionClicks"], doc["targetRelativeTimeMicros"] );
        }
    }
};