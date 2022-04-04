classdef MotorRegulator<handle
    
    properties(Constant)
        P = 10000;
        D = 0;
        Fmax = 255;
        Fmin = -255;
    end
    
    properties
        wantedPosition;
        lastPosition;
    end
    
    methods
        function obj = MotorRegulator()
            obj.wantedPosition = pi/6;
            obj.lastPosition = 0;
        end
        
        function pwm = step(obj,position,wantedPosition)
            obj.lastPosition = position;
            obj.wantedPosition = wantedPosition;
            delta = obj.wantedPosition - obj.lastPosition;
            pwm = MotorRegulator.P * delta;   
            pwm = max(min(pwm,255),-255);
        end
    end
end

