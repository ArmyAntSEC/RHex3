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

template <int N>class RecurringEncoderWrapperHoming: public RecurringTaskBase
{
    private:
        HomingEncoder* encoder;
    public:
        RecurringEncoderWrapperHoming( HomingEncoder* _encoder ): encoder(_encoder)
        {}
        
        virtual void init( unsigned long int _now ) {
            RecurringTaskBase::init();
        }

        virtual void run( unsigned long int ) {
            this->encoder->isr_homing<N>();
        }
};

#endif