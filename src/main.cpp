#ifdef ARDUINO
#include <SerialStream.h>
#include <TaskScheduler.h>
#include <RecurringTaskGroup.h>
#include "oneLeg.h"

MotorPinDefinition leftMotor = { 4, 5, 6, 7, 8, 9 };

OneLeg<0> leg(leftMotor);

TaskScheduler<1> sched;
RecurringTaskGroup<1> recurringGroup( 1000*1000L );

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Log << "Hello World!" << endl;  

  //recurringGroup.addTask( &leg );
  sched.addTask( &recurringGroup );  
}


void loop()
{
  unsigned long nowMicros = micros();
  sched.run(nowMicros);  
}

#else

int main()
{ 
    
}

#endif
