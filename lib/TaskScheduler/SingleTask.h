#ifndef _SINGLETASK_H_
#define _SINGLETASK_H_

#include "Task.h"

class SingleTask: public Task
{
private:
  unsigned long int runTime;
  
public:
  void init( unsigned long int _runTime )
  {
    this->runTime = _runTime;
  }
  
  virtual bool canRun( unsigned long int now)
  {
    if ( now >= this->runTime )
      return true;
    else
      return false;
  }
}

#endif
