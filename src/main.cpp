#ifdef ARDUINO
#include <SerialStream.h>
#include "oneLeg.h"

OneLeg<0> leg(4, 5, 7);

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Log << "Hello World!" << endl;
}


void loop()
{  
}

#else

int main()
{ 
    
}

#endif
