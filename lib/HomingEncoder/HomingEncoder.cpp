#include "HomingEncoder.h"

//Declare the state struct
HomingEncoderState HomingEncoder::stateList[];
const SQ15x16 HomingEncoderState::clicksPerRevolution = 3591.925333/2; //We only use one of the encoder channels for now.
