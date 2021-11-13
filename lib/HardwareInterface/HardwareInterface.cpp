#include "HardwareInterface.h"

#ifndef ARDUINO
long int HardwareInterface::microsSinceBoot = 0;
HardwareInterface::PinMode HardwareInterface::pinModes[];
int HardwareInterface::pinStatuses[];
unsigned int HardwareInterface::EEPROMData[];

#endif