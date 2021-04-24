#include "HomingEncoder.h"

//Declare the state struct
HomingEncoderState * HomingEncoder::stateList[];
const UQ16x16 HomingEncoder::clicksPerRevolution = 3591.925333;
