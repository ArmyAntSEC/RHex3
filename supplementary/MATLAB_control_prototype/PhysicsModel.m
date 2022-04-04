classdef PhysicsModel<handle
    %PHYSICSMODEL Summary of this class goes here
    %   Detailed explanation goes here
    
    properties                
        stepCount;
        systemClock;
        engineOne;
    end
    
    methods
        function obj = PhysicsModel()              
            obj.stepCount = 0;
            obj.systemClock = 0;
            obj.engineOne = EngineModel();               
        end
        
        function step(obj)           
            obj.stepCount = obj.stepCount + 1;            
            obj.systemClock = obj.stepCount * GlobalParams.physicsTimeDelta;
            obj.engineOne.step();                        
        end
    end
end

