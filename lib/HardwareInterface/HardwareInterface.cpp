#include "HardwareInterface.h"

#ifndef ARDUINO
unsigned long HardwareInterface::microsSinceBoot = 0;
unsigned long HardwareInterface::microsToStepOnEachRead = 0;
HardwareInterface::PinMode HardwareInterface::pinModes[];
int HardwareInterface::pinStatuses[];
uint8_t HardwareInterface::EEPROMData[];
HardwareInterface::VoidFcnPtr HardwareInterface::isrList[];

#endif
