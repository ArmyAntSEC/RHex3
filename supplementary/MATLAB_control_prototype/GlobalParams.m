classdef GlobalParams
    %GLOBALPARAMS Summary of this class goes here
    %   Detailed explanation goes here
    
    properties(Constant)
        physicsTimeDelta = 1e-5;
        totalLoops = 2e5;        
    end
    
    properties(Constant)
        shaftToMotorRatio = 74.83;
        encoderClicksPerMotorRotation = 48;
        noLoadMotorMaxSpeedRPS = 162.5;
    end
    
    methods
        function obj = GlobalParams()
        end
    end
end

