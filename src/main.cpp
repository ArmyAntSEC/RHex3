#ifdef ARDUINO
#include <Arduino.h>


void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
    Serial.println("Hello World!");
  }


void loop()
{  
}

#else

int main()
{

}

#endif
