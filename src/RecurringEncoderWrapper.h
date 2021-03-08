#ifndef _RECURRINGENCODERWRAPPER_H_
#define _RECURRINGENCODERWRAPPER_H_

#include "RecurringTask10ms.h"
#include "RecurringTaskEvery.h"
#include "HomingEncoder.h"

class RecurringEncoderWrapper10ms: public RecurringTask10ms
{
    private:
        HomingEncoder* encoder;
    public:
        RecurringEncoderWrapper10ms( HomingEncoder* _encoder ): encoder(_encoder)
        {}

        virtual void run( unsigned long int _now ) {
            RecurringTask::run(_now);

            this->encoder->run();
        }


};

template <int N>class RecurringEncoderWrapperHoming: public RecurringTaskEvery
{
    private:
        HomingEncoder* encoder;
    public:
        RecurringEncoderWrapperHoming( HomingEncoder* _encoder ): encoder(_encoder)
        {}
        
        virtual void init( unsigned long int _now ) {
            RecurringTaskEvery::init();
        }

        virtual void run( unsigned long int ) {
            this->encoder->isr_homing<N>();
        }
};

#endif