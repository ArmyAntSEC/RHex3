classdef TraceLogMat<handle
    %TRACELOGMAT Summary of this class goes here
    %   Detailed explanation goes here
    
    properties(Constant)
        elementsToAdd = 100;
    end
    
    properties
        data;
        nextRow;
    end
    
    methods
        function obj = TraceLogMat()
            obj.data = zeros(TraceLogMat.elementsToAdd,1);
            obj.nextRow = 1;
        end
        
        function append(obj,value)
            if ( obj.nextRow > numel(obj.data) )
                obj.data = [obj.data; zeros(TraceLogMat.elementsToAdd,1)];                    
            end
            obj.data(obj.nextRow) = value;
            obj.nextRow = obj.nextRow + 1;
        end
    end
end

