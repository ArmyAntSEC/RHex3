classdef EngineModel<handle
    
    properties(SetAccess=private)
        motorPositionRev;
        shaftPositionRev; 
        encoderOutputOne;
        encoderOutputTwo;
        inputPWM;
        
        motorPositionRevLog;
        shaftPositionRevLog; 
        encoderOutputOneLog;
        encoderOutputTwoLog;
        inputPWMLog;
    end
    
    methods
        function obj = EngineModel()
            obj.motorPositionRev = 0;
            obj.shaftPositionRev = 0;
            obj.encoderOutputOne = 0;
            obj.encoderOutputTwo = 0;
            obj.inputPWM = 0;      
            
            obj.motorPositionRevLog = TraceLogMat();
            obj.shaftPositionRevLog = TraceLogMat(); 
            obj.encoderOutputOneLog = TraceLogMat();
            obj.encoderOutputTwoLog = TraceLogMat();
            obj.inputPWMLog = TraceLogMat();
        end
        
        function step(obj)
            motorSpeedRPS = GlobalParams.noLoadMotorMaxSpeedRPS * obj.inputPWM/256;
            obj.motorPositionRev = obj.motorPositionRev + GlobalParams.physicsTimeDelta*motorSpeedRPS;
            obj.shaftPositionRev = obj.motorPositionRev / GlobalParams.shaftToMotorRatio;
            obj.encoderOutputOne = sin(obj.motorPositionRev*GlobalParams.encoderClicksPerMotorRotation*2*pi/4)>0;
            obj.encoderOutputTwo = cos(obj.motorPositionRev*GlobalParams.encoderClicksPerMotorRotation*2*pi/4)>0;            

            obj.motorPositionRevLog.append( obj.motorPositionRev );
            obj.shaftPositionRevLog.append( obj.shaftPositionRev );
            obj.encoderOutputOneLog.append( obj.encoderOutputOne );
            obj.encoderOutputTwoLog.append( obj.encoderOutputTwo );
            obj.inputPWMLog.append( obj.inputPWM );
        end
        
        function setInputPWM( obj, pwm )
            assert ( pwm >-256 && pwm < 256 );
            obj.inputPWM = pwm;
        end
    end
end

