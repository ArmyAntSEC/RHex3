%% Lock and load
clc;
clear;

%% Do the setup
physicsModel = PhysicsModel();
systemModel = SystemModel();

%% Start the loop
t = tic;
for ii = 1:GlobalParams.totalLoops                                   
    physicsModel.step();           
    systemModel.step(physicsModel);    
end

endTime = toc(t);
fprintf ( "Time to run: %0.2fs. Loops/s: %0.2f.\n", endTime, GlobalParams.totalLoops/endTime );

%% Plot data of interest
physicsTimeScale = (1:GlobalParams.totalLoops)*GlobalParams.physicsTimeDelta;

ax1 = subplot ( 2,2,1);
plot ( physicsTimeScale, physicsModel.engineOne.motorPositionRevLog.data/GlobalParams.shaftToMotorRatio, '-', ...
    physicsTimeScale, systemModel.encoderReaderOne.lastMotorPositionRevLog.data/GlobalParams.shaftToMotorRatio, '.-', ...
    physicsTimeScale, systemModel.encoderReaderOne.lastMotorPositionRevFilteredLog.data/GlobalParams.shaftToMotorRatio, '.-',...
    physicsTimeScale(1:10:end), systemModel.motorScheduler.wantedShaftPositionRevLog.data, '--');
title ( 'Actual and Encoder measured shaft position [rev]' );

ax2 = subplot ( 2,2,2);
plot ( physicsTimeScale, physicsModel.engineOne.inputPWMLog.data, '.-' );
title ( 'Driver input PWM value' );

ax3 = subplot ( 2,2,3);
plot ( physicsTimeScale, physicsModel.engineOne.encoderOutputOneLog.data, ...
    physicsTimeScale, physicsModel.engineOne.encoderOutputTwoLog.data );
title ( 'Encoder output' );


linkaxes( [ax1 ax2 ax3], 'x' );

