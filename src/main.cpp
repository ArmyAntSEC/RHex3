#include <Arduino.h>

#define MOTOR_EN1 4
#define MOTOR_EN2 5
#define MOTOR_CS A0
#define ENCODER_1 2
#define ENCODER_2 3
#define OPTO 7
#define MOTOR_PWM 6

#include <HomingEncoder.h>

HomingEncoder encoder1;

void setup()
{
  Serial.begin( 115200 );

  encoder1.init<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );
  pinMode ( MOTOR_EN1, OUTPUT );
  pinMode ( MOTOR_EN2, OUTPUT );
  pinMode ( MOTOR_PWM, OUTPUT );
  
  digitalWrite ( MOTOR_EN1, 1 );
  digitalWrite ( MOTOR_EN2, 0 );
}

int counter = 0;

const int logLen = 160;
long int posLog[logLen];

unsigned long int nextCall = 0;

void loop()
{
  if ( millis() > nextCall ) {
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
    nextCall += 10;
  }
} 
