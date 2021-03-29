%% Lock and Load
clear;
clc;

%% Open the device
setPoint = 7000;
dev = serialport ( 'COM4', 115200 ); 
dev.configureTerminator( 'CR' );
pause(2);
headerLine = dev.readline();
disp ( headerLine );

%%
dev.write ( 2, 'uint8' );
dev.write ( 10000, 'single' );
dev.write ( setPoint, 'single' );
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

subplot ( 3, 1, 1 );
plot ( data.time, data.pos );
subplot ( 3, 1, 2 );
plot ( data.time, data.speed, data.time, ones(size(data.speed))*setPoint );
subplot ( 3, 1, 3 );
plot ( data.time, data.power );

%% And a second command
dev.write ( 0, 'uint8' );
dev.write ( 1000, 'single' );
pause(3);
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

subplot ( 2, 1, 1 );
plot ( data.time, data.pos );
subplot ( 2, 1, 2 );
plot ( data.time, data.speed );


%%
clear('dev');
disp ( 'Done' );