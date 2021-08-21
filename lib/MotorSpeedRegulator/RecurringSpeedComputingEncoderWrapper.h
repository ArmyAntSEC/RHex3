#ifndef _RECURRINGENCODERWRAPPER_H_
#define _RECURRINGENCODERWRAPPER_H_

#include "Task.h"
#include "RecurringTaskBase.h"
#include "HomingEncoder.h"

class EncoderWrapperComputeSpeedTask: public Task
{
    private:
        HomingEncoder* encoder;
    public:
        EncoderWrapperComputeSpeedTask( HomingEncoder* _encoder ): encoder(_encoder)
        {}

        virtual void run( unsigned long int _now ) {            
            this->encoder->run();
        }
};

#endif