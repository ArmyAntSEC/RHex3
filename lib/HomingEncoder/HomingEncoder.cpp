#include "HomingEncoder.h"

//Declare the state struct
HomingEncoder HomingEncoderFactory::stateList[];
const SQ15x16 HomingEncoder::clicksPerRevolution = 3591.925333/2; //We only use one of the encoder channels for now.
