#ifndef BGACTIVITY_ISTARTSTRATEGY_H
#define BGACTIVITY_ISTARTSTRATEGY_H

#include <jni.h>
#include "../../android/Intent.h"

class IStartStrategy : public virtual DeferredGC::VirtualDestructor {
public:
    virtual long delayTimeMills()=0;

    virtual bool start(jobject context, Intent& intent)=0;

    virtual const char* getName()=0;
};


#endif //BGACTIVITY_ISTARTSTRATEGY_H
