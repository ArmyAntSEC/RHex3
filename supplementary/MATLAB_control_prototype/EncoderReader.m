classdef EncoderReader<handle
    properties ( Constant )
        filterConstant = 100;
    end
    
    properties(Access=private)
        lastValueOne;
        lastValueTwo;
        lastMotorPositionRev;         
        
        lastMotorPositionRevFiltered;        
        
        transitionsMatrix                
    end
    
    properties
        lastMotorPositionRevLog; 
        lastMotorPositionRevFilteredLog;
    end
    
    methods
        function obj = EncoderReader()
            obj.lastValueOne = 0;
            obj.lastValueTwo = 1;
            obj.lastMotorPositionRev = 0;
            obj.lastMotorPositionRevFiltered = 0;
            
            obj.transitionsMatrix = zeros(2,2,2,2);
            obj.transitionsMatrix(2,2,1,2) = 1;
            obj.transitionsMatrix(1,2,1,1) = 1;
            obj.transitionsMatrix(1,1,2,1) = 1;
            obj.transitionsMatrix(2,1,2,2) = 1;
            obj.transitionsMatrix(2,2,2,1) = -1;
            obj.transitionsMatrix(2,1,1,1) = -1;
            obj.transitionsMatrix(1,1,1,2) = -1;
            obj.transitionsMatrix(1,2,2,2) = -1;
            
            obj.lastMotorPositionRevLog = TraceLogMat;
            obj.lastMotorPositionRevFilteredLog = TraceLogMat;
        end
        
        function step(obj,valueOne,valueTwo)
            stepDir = -obj.transitionsMatrix( ...
                obj.lastValueOne+1, obj.lastValueTwo+1, ...
                valueOne+1, valueTwo+1 );
            stepRevFraction = stepDir / GlobalParams.encoderClicksPerMotorRotation;
            
            obj.lastValueOne = valueOne;
            obj.lastValueTwo = valueTwo;
            
            obj.lastMotorPositionRev = obj.lastMotorPositionRev + stepRevFraction;
            obj.lastMotorPositionRevFiltered = ...
                (obj.lastMotorPositionRevFiltered*(EncoderReader.filterConstant-1) + ...
                obj.lastMotorPositionRev) / EncoderReader.filterConstant;
          
            
            obj.lastMotorPositionRevLog.append( obj.lastMotorPositionRev );
            obj.lastMotorPositionRevFilteredLog.append( obj.lastMotorPositionRevFiltered );
        end
        
        function pos = getShaftPositionRev(obj)
            pos = obj.lastMotorPositionRevFiltered/GlobalParams.shaftToMotorRatio;
        end
    end
end

