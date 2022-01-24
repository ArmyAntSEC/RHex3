#include "HomingEncoder.h"

//Declare the state struct
HomingEncoder HomingEncoderFactory::stateList[];
const int HomingEncoder::clicksPerRevolution = (int)3591.925333/2; //We only use one of the encoder channels for now.
