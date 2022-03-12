#ifdef ARDUINO
#include <HardwareClock.h>
#include <HardwareInterrupts.h>
#include <HardwarePins.h>
#include <SerialStream.h>
#include <TaskScheduler.h>
#include <TaskAwareDelay.h>
#include <RecurringTaskGroup.h>
#include <oneLeg.h>


HardwareClock hwClock;
HardwareInterrupts hwInterrupts;
HardwarePins hwPins;

TaskScheduler<1> sched;
RecurringTaskGroup<1> recurringGroup( 10*1000L );

TaskAwareDelay awareDelay(&hwClock, &sched);

OneLeg leftLeg;

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Log << "Hello World!" << endl;  
  
  leftLeg.config();
  
  sched.addTask( &recurringGroup );
  recurringGroup.addTask( &leftLeg.regulator );
  
  Log << "Starting" << endl;
  awareDelay.delayMicros( 1000*1000L );
  Log << "Done: " << leftLeg.linPos.getLinearPosition() << endl;

  leftLeg.driver.setMotorPWM( 0 );
}


void loop()
{
  unsigned long nowMicros = micros();
  //sched.run(nowMicros);  
}

#else

int main()
{ 
    
}

#endif
