#ifndef BGACTIVITY_STARTSTRATEGYNORMAL_H
#define BGACTIVITY_STARTSTRATEGYNORMAL_H

#include "IStartStrategy.h"


class StartStrategyNormal: public IStartStrategy {
public:
    long delayTimeMills();
    bool start(jobject context, Intent& intent);
    const char* getName();

    ~StartStrategyNormal();

};


#endif //BGACTIVITY_STARTSTRATEGYNORMAL_H
