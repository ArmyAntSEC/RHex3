classdef SystemModel<handle
    %SYSTEMMODEL Summary of this class goes here
    %   Detailed explanation goes here
    
    properties                
        stepCount;                
        encoderReaderOne;
        motorRegulator;
        motorScheduler;
    end
    
    methods
        function obj = SystemModel()                          
            obj.stepCount = 0;            
            obj.encoderReaderOne = EncoderReader();
            obj.motorRegulator = MotorRegulator();
            obj.motorScheduler = MotorScheduler();
        end
        
        function step(obj, physicsModel)           
            obj.stepCount = obj.stepCount + 1;
            
            %Read the encoder
            encoderValueOne = physicsModel.engineOne.encoderOutputOne;
            encoderValueTwo = physicsModel.engineOne.encoderOutputTwo;
            obj.encoderReaderOne.step( encoderValueOne, encoderValueTwo );
            
            % Only run regulator every 10 iterations to save processor
            % power.
            if ( rem( obj.stepCount, 10 ) == 0 )
                % Read the wanted position
                systemClock = physicsModel.systemClock;
                wantedShaftPosition = obj.motorScheduler.getWantedShaftPosRev( systemClock );
                
                %Regulate the motor power
                shaftPosition = obj.encoderReaderOne.getShaftPositionRev();
                pwm = obj.motorRegulator.step(shaftPosition, wantedShaftPosition);
                physicsModel.engineOne.setInputPWM( pwm );
            end
            
        end
    end
end

