#ifndef BGACTIVITY_STARTSTRATEGYALARMIMPL_H
#define BGACTIVITY_STARTSTRATEGYALARMIMPL_H

#include "IStartStrategy.h"

class StartStrategyAlarmImpl : public IStartStrategy{
public:
    long delayTimeMills();
    bool start(jobject context, Intent& intent);
    const char* getName();

    ~StartStrategyAlarmImpl();
};


#endif //BGACTIVITY_STARTSTRATEGYALARMIMPL_H
