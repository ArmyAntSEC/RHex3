function motionDynamics()
    params.totalRobotMass = 2; %kg
    params.motorStallTorque = 17*9.8/100; %Nm
    params.motorFreeSpeed = 130/9.5493; %rad/s
    params.legMaxLength = 75/1000; %m
    params.totalMotors = 6;
    
    fprintf( 'Vertical stall acceleration from lying down: %0.2f m/s^2\n', ...
        verticalAccForTorqueAndLegAngle( params.motorStallTorque*params.totalMotors*0.2, 0, params ) );
    fprintf( 'MaxTorqueLyingDown: %0.2f Nm\n', ...
        maxTorqueForVerticalSpeedAndAngle( 0, 0, params ) );
    
    [t, y] = ode45( @(t,y)stepFunction(t,y,params), [0 0.5], [0 0] );
    
    verticalSpeed = y(:,1);
    verticalHeight = y(:,2);
    verticalSpeed2 = [0; diff(verticalHeight)./diff(t)];
    
    legAngle = asin( verticalHeight ./ params.legMaxLength );
    axelRotationSpeed = verticalSpeed ./ (params.legMaxLength * cos( legAngle ) );
    legAngle2 = [cumsum(axelRotationSpeed(1:end-1) .* diff(t)); nan];
    
    maxTorque = 0.2*interp1( [0 params.motorFreeSpeed], ...
        [params.totalMotors*params.motorStallTorque 0], axelRotationSpeed );
    verticalAcceleration = [0; diff( verticalSpeed ) ./ diff(t)];
    
    subplot ( 3, 2, 1 );
    plot ( t, [verticalSpeed verticalSpeed2]*100 );
    ylabel ( 'Vertical Speed [cm/s]' );
    
    subplot ( 3, 2, 2 );
    plot ( t, verticalHeight*100 );
    ylabel ( 'Vertical Height [cm]' );
    
    subplot ( 3, 2, 3 );
    plot ( t, [legAngle legAngle2]*180/pi );
    ylabel ( 'Leg angle [°]' );
    
    subplot ( 3, 2, 4 );
    plot ( t, axelRotationSpeed*180/pi );
    ylabel ( 'Axel rotation speed [°/s]' );
    
    subplot ( 3, 2, 5 );
    plot ( t, maxTorque );
    ylabel ( 'Max torque [Nm]' );
    
    subplot ( 3, 2, 6 );
    plot ( t, verticalAcceleration );
    ylabel ( 'verticalAcceleration [m/s^2]' );
    
end

function [ dydt ] = stepFunction ( ~, y, params )
    verticalSpeed = y(1);
    verticalHeight = y(2);
    
    legAngle = asin( verticalHeight / params.legMaxLength );
    
    torque = maxTorqueForVerticalSpeedAndAngle( verticalSpeed, legAngle, params );
    
    verticalAcceleration = verticalAccForTorqueAndLegAngle( torque, legAngle, params );
    dydt(1,1) = verticalAcceleration;
    dydt(2,1) = verticalSpeed;
    
end


% Compute vertical acceleration for torque and leg angle
function [verticalAcceleration] = verticalAccForTorqueAndLegAngle( torque, legAngle, params )
    legLeverageLength = params.legMaxLength * cos( legAngle );
    legVerticalForce = torque / legLeverageLength;
    totalNetVerticalForce = legVerticalForce - params.totalRobotMass*9.8;
    verticalAcceleration = totalNetVerticalForce / params.totalRobotMass;
end

% Compute max torque for given vertical speed
function [maxTorque] = maxTorqueForVerticalSpeedAndAngle( verticalSpeed, legAngle, params )
    legLeverageLength = params.legMaxLength * cos( legAngle );
    axelRotationSpeed = verticalSpeed / legLeverageLength;
    maxTorque = interp1( [0 params.motorFreeSpeed], ...
        [params.totalMotors*params.motorStallTorque 0], axelRotationSpeed );
    maxTorque = 0.2*maxTorque; %Add an 80% safety margin.
end







