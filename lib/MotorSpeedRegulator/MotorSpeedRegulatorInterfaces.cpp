#include "MotorSpeedRegulatorInterfaces.h"

SerialStream& operator<< ( SerialStream& stream, const MotorCommanderGoal& goal )
{    
    stream << "[ TargetPos: " << goal.targetPositionClicks << ", TargetRelTime: " << goal.targetRelativeTimeMicros << " ]";    
    return stream;
}
