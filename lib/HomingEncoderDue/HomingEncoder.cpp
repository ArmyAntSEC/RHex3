#include "HomingEncoder.h"

//Declare the state struct
HomingEncoderState * HomingEncoder::stateList[];
const SQ15x16 HomingEncoder::clicksPerRevolution = 3591.925333;
