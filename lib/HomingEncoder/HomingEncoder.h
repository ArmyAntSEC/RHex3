#ifndef _HOMINGENCODER_H_
#define _HOMINGENCODER_H_

#include <FixedPointsCommon.h>
#include "RotationPositionWithLaps.h"
#include <HardwareInterface.h>
#include <VolatileRotationPositionWithLaps.h>
#define MAX_ENCODERS_SUPPORTED 6

class HomingEncoder
{
private:
  static const SQ15x16 clicksPerRevolution;
  const int speedTimeConstant = 10;

  int encoderPin1;
  int encoderPin2;
  int homingPin;
  
  VolatileRotationPositionWithLaps position;

  volatile bool is_homed;
  //TODO: Use the above isr-safe object to also handle these things to that we can handle pos at last home in a more robust manner.
  //volatile long int pos_at_last_home;
  //volatile long int laps_at_last_home;
  VolatileRotationPositionWithLaps positionAtLastHome;

  //Also use the above object here as well.
  long int last_position;
  unsigned long int last_position_timestamp_micros;
  SQ15x16 speed_cps;
  SQ15x16 speed_cps_filtered;

public:
  void config(unsigned int _encoderPin1,
              unsigned int _encoderPin2, unsigned int _breakerPin,
              int _offset)
  {    
    encoderPin1 = _encoderPin1;
    encoderPin2 = _encoderPin2;
    homingPin = _breakerPin;

    HardwareInterface::configurePin(encoderPin1, HardwareInterface::INPUT_PULLUP);
    HardwareInterface::configurePin(encoderPin2, HardwareInterface::INPUT_PULLUP);
    HardwareInterface::configurePin(homingPin, HardwareInterface::INPUT_PULLUP);

    HardwareInterface::disableInterrupts();
    position = VolatileRotationPositionWithLaps();

    is_homed = false;
    positionAtLastHome = VolatileRotationPositionWithLaps();

    last_position = 0;
    last_position_timestamp_micros = 0;
    speed_cps = 0;
    speed_cps_filtered = 0;
    
    HardwareInterface::enableInterrupts();
  }

  RotationPositionWithLaps getPosition()
  {
    return position.getNonVolatileCopy();
  }

  long int getRawPos()
  {
    HardwareInterface::disableInterrupts();
    long int pos = getRawPosISR();
    HardwareInterface::enableInterrupts();
    return pos;
  }

  long int getRawPosISR()
  {
    return position.getClickPositionISR();
  }

  void handleOverflow()
  {    
    HardwareInterface::disableInterrupts();
    position.reduceToMinimalForm();
    HardwareInterface::enableInterrupts();
  }

  //Should be called once every 10ms to compute the speed.
  virtual void run()
  {

    //Calculate the max range for values:
    //Max rpm: 3
    //Max clicks per second: 3*3500 = 10000
    //Max clicks per 0.01 s = 100

    //Min clicks per second 4700
    //Min clicks per 0.01s = 47

    unsigned long int nowU = HardwareInterface::getMicrosecondsSinceBoot();

    int thisPos = this->getRawPos();
    int lastPos = last_position;
    unsigned long int lastTimeU = last_position_timestamp_micros;

    SQ15x16 posDelta = positionPositiveDifference(thisPos, lastPos);
    last_position = thisPos;

    SQ15x16 timeDelta = nowU - lastTimeU;
    last_position_timestamp_micros = nowU;

    SQ15x16 speedCPS;
    if (timeDelta == 0)
    {
      speedCPS = 0;
    }
    else
    {
      //First pre-scale by 100, then divide, then scale by 10k for a total of 1e6.
      speedCPS = 10000 * ((100 * posDelta) / timeDelta);
    }

    last_position = thisPos;
    last_position_timestamp_micros = nowU;
    speed_cps = speedCPS;
    speed_cps_filtered =
        ((speed_cps_filtered +
          speed_cps / (this->speedTimeConstant - 1)) /
         this->speedTimeConstant) *
        (this->speedTimeConstant - 1);

    //Make sure that we handle any overflows.
    handleOverflow();
  }

  static long int positionPositiveDifference(long int pos1, long int pos2)
  {
    if (pos1 >= pos2)
      return pos1 - pos2;
    else
      return clicksPerRevolution.getInteger() + pos1 - pos2;
  }

  long int getSpeedCPS()
  {
    return speed_cps.getInteger();
  }

  long int getSpeedCPSFiltered()
  {
    return speed_cps_filtered.getInteger();
  }

  void unHome()
  {
    HardwareInterface::disableInterrupts();
    is_homed = false;
    HardwareInterface::enableInterrupts();
  }

  void forceHomed()
  {
    HardwareInterface::disableInterrupts();
    forceHomedISR();
    HardwareInterface::enableInterrupts();
  }

  void forceHomedISR()
  {
    is_homed = true;
    positionAtLastHome = position;
    position = VolatileRotationPositionWithLaps();
  }

  bool isHomed()
  {
    HardwareInterface::disableInterrupts();
    bool _is_homed = isHomedISR();
    HardwareInterface::enableInterrupts();
    return _is_homed;
  }

  bool isHomedISR()
  {    
    return is_homed;
  }

  RotationPositionWithLaps getPosAtLastHome()
  {
    return positionAtLastHome.getNonVolatileCopy();
  }

  void incrementRawPosISR()
  {
    position.incrementMe();
  }

  unsigned int getHomingPinValue()
  {
    return HardwareInterface::getDigitalValueFromPin(homingPin);
  }
};

class HomingEncoderFactory
{
public:
  static HomingEncoder stateList[MAX_ENCODERS_SUPPORTED];

  template <int N>
  static HomingEncoder *config(
      unsigned int encoderPin1, unsigned int encoderPin2,
      unsigned int homingPin, int offset)
  {
    static_assert(N < MAX_ENCODERS_SUPPORTED);
    HomingEncoder *state = &stateList[N];

    state->config(encoderPin1, encoderPin2, homingPin, offset);

    //Only trigger homing on rising or we home twice per rotation
    HardwareInterface::attachAnInterrupt(homingPin, isr_homing<N>, HardwareInterface::FALLING);
    HardwareInterface::attachAnInterrupt(encoderPin1, isr_encoder<N>, HardwareInterface::CHANGE);

    return state;
  }

  template <int N>
  static void isr_encoder(void)
  {
    HomingEncoder *state = &stateList[N];
    state->incrementRawPosISR();
  }

  template <int N>
  static void isr_homing(void)
  {
    HomingEncoder *state = &stateList[N];

    
    if (!state->isHomedISR() && state->getRawPosISR() > 200)
    { //Do some debouncing.
      state->forceHomedISR();
    }
  }
};

#endif
