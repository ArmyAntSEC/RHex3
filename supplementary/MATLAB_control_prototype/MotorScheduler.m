classdef MotorScheduler    
    properties
        wantedShaftPositionRevLog;
        cycleTime;
        slowTime;
        slowPhase;
        angleOffset;
        slowTimeDelta;
        angleOffsetDelta;
    end
    
    methods
        function obj = MotorScheduler()
            obj.wantedShaftPositionRevLog = TraceLogMat;
            obj.cycleTime = 1;
            obj.slowTime = obj.cycleTime*0.5;
            obj.slowPhase = 2*pi*1/3;
            obj.angleOffset = pi/4;
            obj.slowTimeDelta = 0.25;
            obj.angleOffsetDelta = pi/4;
        end
    end
    
    methods
        function wantedShaftPosRev = getWantedShaftPosRev(obj,systemTime)            
            locationInTimeLoop = rem(systemTime,obj.cycleTime);            
            timeLoopOffset = floor(systemTime/obj.cycleTime);
            
            time = [0 (obj.cycleTime/2-(obj.slowTime+obj.slowTimeDelta)/2) (obj.cycleTime/2+(obj.slowTime+obj.slowTimeDelta)/2) obj.cycleTime];
            angle = [-pi -obj.slowPhase/2 obj.slowPhase/2 pi] + obj.angleOffset + obj.angleOffsetDelta;
            wantedShaftPosAngle = interp1( time,angle, locationInTimeLoop) + 2*pi*timeLoopOffset;
            wantedShaftPosRev = wantedShaftPosAngle/(2*pi);
            
            obj.wantedShaftPositionRevLog.append( wantedShaftPosRev );
        end
    end
end

