#ifndef _RECURRINGENCODERWRAPPER_H_
#define _RECURRINGENCODERWRAPPER_H_

#include "RecurringTask10ms.h"
#include "HomingEncoder.h"

class RecurringEncoderWrapper: public RecurringTask10ms
{
    private:
        HomingEncoder* encoder;
    public:
        RecurringEncoderWrapper( HomingEncoder* _encoder ): encoder(_encoder)
        {}

        virtual void run( unsigned long int ) {
            this->encoder->run();
        }


};

#endif