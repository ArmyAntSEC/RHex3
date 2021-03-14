%% Lock and Load
clear;
clc;

%% Open the device
dev = serialport ( 'COM4', 115200 ); 
dev.configureTerminator( 'CR' );
pause(2);
dev.write ( [0 1000], 'single' );
pause(2);
headerLine = dev.readline();
dev.read(1,'uint8'); %Throw away the newline
disp ( headerLine );
VariableNames = split( headerLine, ', ' );
data = cell2table( cell(0,numel(VariableNames)), 'VariableNames', VariableNames );

%% Now read the data
bytesToRead = dev.NumBytesAvailable;
floatsToRead = numel(VariableNames)*floor(bytesToRead/numel(VariableNames)/4);
rawData = dev.read(floatsToRead, 'uint32' );
rawData = reshape( rawData, 3, [] )';

%% Now look at the data
subplot ( 3, 1, 1 );
plot ( data.Time, data.Speed, [0 max(data.Time)], targetSpeed*[1 1] );
title ( 'Speed' );
subplot ( 3, 1, 2 );
plot ( data.Time, data.Power, data.Time(1:end-1), diff(data.Speed)/5 );
title ( 'Power' );
subplot ( 3, 1, 3 );
plot ( data.Time, data.Position, [0 max(data.Time)], 3000*[1 1] );
title ( 'Postion' );
fprintf ( 'End position: %d\n', data.Position(end) );

clear('dev');
disp ( 'Done' );