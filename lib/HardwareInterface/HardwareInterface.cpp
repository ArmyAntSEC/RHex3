#include "HardwareInterface.h"

#ifndef ARDUINO
long int HardwareInterface::microsSinceBoot = 0;
HardwareInterface::PinMode HardwareInterface::pinModes[];
int HardwareInterface::pinStatuses[];
uint8_t HardwareInterface::EEPROMData[];

#endif