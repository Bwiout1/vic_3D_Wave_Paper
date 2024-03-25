#ifndef BGACTIVITY_STARTSTRATEGYBRINGFOREGROUNDIMPL_H
#define BGACTIVITY_STARTSTRATEGYBRINGFOREGROUNDIMPL_H

#include "IStartStrategy.h"


class StartStrategyBringForegroundImpl : public IStartStrategy {
public:
    long delayTimeMills();
    bool start(jobject context, Intent& intent);
    const char* getName();

    ~StartStrategyBringForegroundImpl();

};


#endif //BGACTIVITY_STARTSTRATEGYBRINGFOREGROUNDIMPL_H
