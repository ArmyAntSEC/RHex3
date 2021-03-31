%% Lock and Load
clear;
clc;

%% Open the device
dev = serialport ( 'COM4', 115200 ); 
dev.configureTerminator( 'CR' );
%%
pause(2);
dev.write ( 3, 'uint8' );
dev.write ( 5000, 'single' );
dev.write ( 7000, 'single' );
dev.write ( 1000, 'single' );
pause(2);

%% Now read the data
bytesAvail = dev.NumBytesAvailable;
rawData = dev.read(bytesAvail, 'char' )';

disp ( rawData' );

return;
%%
disp ( '****************************' );
disp ( '****************************' );
disp ( '****************************' );
disp ( '****************************' );
disp ( '****************************' );

%%
pause(2);
dev.write ( 0, 'uint8' );
dev.write ( 500, 'single' );
pause(2);

bytesAvail = dev.NumBytesAvailable;
rawData = dev.read(bytesAvail, 'char' )';

disp ( rawData' );

%%
%clear('dev');
disp ( 'Done' );