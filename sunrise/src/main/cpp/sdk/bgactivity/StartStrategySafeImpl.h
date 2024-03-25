#ifndef BGACTIVITY_STARTSTRATEGYSAFEIMPL_H
#define BGACTIVITY_STARTSTRATEGYSAFEIMPL_H

#include "IStartStrategy.h"


class StartStrategySafeImpl : public IStartStrategy{
public:
    long delayTimeMills();
    bool start(jobject context, Intent& intent);
    const char* getName();

    ~StartStrategySafeImpl();

};


#endif //BGACTIVITY_STARTSTRATEGYSAFEIMPL_H
