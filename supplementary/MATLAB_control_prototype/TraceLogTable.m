classdef TraceLog<handle
    %TRACELOG Summary of this class goes here
    %   Detailed explanation goes here
    properties(Constant)
        RowsToAllocate = 1000;
    end
    
    properties
        log;
        nextRow;
    end
    
    methods
        function [obj] = TraceLog(variables)
            [varTypes{1:numel(variables)}] = deal('double');
            obj.log = table( 'Size', [TraceLog.RowsToAllocate numel(variables)], ...
                'VariableTypes', varTypes, 'VariableNames', variables );
            obj.nextRow = 1;
        end
        
        function appendValues( obj, data )
            if ( obj.nextRow > height(obj.log) )
                totalHeight = height(obj.log) + TraceLog.RowsToAllocate;
                obj.log(totalHeight, :) = zeros(1,width(obj.log));
            end
            
            fieldNames = fields(data);
            for ii = fieldNames'
                fieldName = ii{1};
                obj.log(obj.nextRow,fieldName) = data.(fieldName);
            end
            obj.nextRow = obj.nextRow + 1;
        end
    end
end

