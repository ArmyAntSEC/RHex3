%% Lock and Load
clear;
clc;

%% Open the device
posToStopAt = 10000;
maxSpeed = 7000;
timeToMove = 1800;
dev = serialport ( 'COM4', 115200 ); 
dev.configureTerminator( 'CR' );
pause(5);
headerLine = strtrim(dev.readline());
dev.read(1,'uint8'); %Throw away the newline
disp ( headerLine );
VariableNames = split( headerLine, ', ' );

%% Now read the data
bytesAvail = dev.NumBytesAvailable;
floatsToRead = numel(VariableNames)*floor(bytesAvail/numel(VariableNames)/4);
rawData = dev.read(floatsToRead, 'single' )';
rawData = reshape( rawData, numel(VariableNames), [] )';

%% Format the data
data = cell2table( num2cell(rawData), 'VariableNames', VariableNames );

subplot ( 4, 1, 1 );
plot ( data.time, data.pos );
subplot ( 4, 1, 2 );
plot ( data.time, data.speed );
subplot ( 4, 1, 3 );
plot ( data.time, data.power );

subplot ( 4, 1, 4 );
plot ( data.time, data.targetSpeed );

%%
clear dev;
disp ( 'Done' );