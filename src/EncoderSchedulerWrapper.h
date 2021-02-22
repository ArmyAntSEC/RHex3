#ifndef _ENCODERSCHEDULERWRAPPER_H_
#define _ENCODERSCHEDULERWRAPPER_H_

#include <RecurringTask.h>

class EncoderSchedulerWrapper: public RecurringTask
{
private:
  HomingEncoder* encoder;
public:
  EncoderSchedulerWrapper( HomingEncoder* _encoder )
  {
    this->encoder = _encoder;
  }
  sa

    
  
  
};

#endif