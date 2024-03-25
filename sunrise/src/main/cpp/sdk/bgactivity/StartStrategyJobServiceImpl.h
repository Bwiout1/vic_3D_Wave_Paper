#ifndef BGACTIVITY_STARTSTRATEGYJOBSERVICEIMPL_H
#define BGACTIVITY_STARTSTRATEGYJOBSERVICEIMPL_H

#include "IStartStrategy.h"


class StartStrategyJobServiceImpl : public IStartStrategy{
public:
    long delayTimeMills();
    bool start(jobject context, Intent& intent);
    const char* getName();

    ~StartStrategyJobServiceImpl();

};


#endif //BGACTIVITY_STARTSTRATEGYJOBSERVICEIMPL_H
