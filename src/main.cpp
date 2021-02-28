#include <Arduino.h>

#define MOTOR_EN1 4
#define MOTOR_EN2 5
#define MOTOR_CS A0
#define ENCODER_2 2
#define ENCODER_1 3
#define OPTO 7
#define MOTOR_PWM 6

#include <HomingEncoder.h>
#include <TaskScheduler.h>
#include <SerialStream.h>
#include "MotorDriver.h"
#include "MotorSpeedController.h"

HomingEncoder encoder1;

TaskScheduler sched;

MotorDriver driver;

MotorSpeedController controller ( 10 );

class PositionBeacon: public RecurringTask
{
  public:
    PositionBeacon( unsigned int _rate): RecurringTask( _rate ) {}

    virtual void run( unsigned long int now )
    {
      Log << "Position: " << encoder1.readCompensated() << endl;
    }
};
PositionBeacon beacon(100);

void setup()
{
  Serial.begin( 115200 );

  encoder1.init<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );
  pinMode ( MOTOR_EN1, OUTPUT );
  pinMode ( MOTOR_EN2, OUTPUT );
  pinMode ( MOTOR_PWM, OUTPUT );

  digitalWrite ( MOTOR_EN1, 1 );
  digitalWrite ( MOTOR_EN2, 0 );

  Log << "Hello World!" << endl;

  driver.init( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM, MOTOR_CS );


  Log << "Start pos: " << encoder1.readCompensated() << ", " << encoder1.isHomed() << endl;
  driver.setMotorPWM( 64 );
  delay(1000);
  Log << "Mid pos: " << encoder1.readCompensated() << ", " << encoder1.isHomed() << endl;
  driver.setMotorPWM( -64 );
  delay(1000);
  Log << "End pos: " << encoder1.readCompensated() << ", " << encoder1.isHomed() << endl;
  driver.setMotorPWM( 0 );

  controller.init( millis(), &driver, &encoder1 );
  controller.setDestinationPositionAndSpeed( 1500, 64 );

  sched.add( &controller );

  beacon.init(  millis() );
  sched.add( &beacon );
}


void loop()
{
  sched.run();
}


//*********************************
// ** A function to test the leg dynamics. Should be called once every 10ms
//***********************************
int counter = 0;
const int logLen = 160;
long int posLog[logLen];

void testLegDynamics ( void )
{
  if ( counter == 0 )
    analogWrite ( MOTOR_PWM, 255 );
  if ( counter == logLen/2 )
    analogWrite ( MOTOR_PWM, 0 );
  if ( counter == logLen ) {
    Serial.println ( millis() );
    for ( int i = 0; i < logLen; i++ ) {
      Serial.print ( i );
      Serial.print ( '\t' );
      Serial.println ( posLog[i] );
    }
  }
  if ( counter < logLen ) {
    posLog[counter] = encoder1.readCompensated();
  }
  
  counter++;
}

