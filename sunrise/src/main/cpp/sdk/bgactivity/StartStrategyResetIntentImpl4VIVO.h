#ifndef BGACTIVITY_STARTSTRATEGYRESETINTENTIMPL4VIVO_H
#define BGACTIVITY_STARTSTRATEGYRESETINTENTIMPL4VIVO_H

#include "IStartStrategy.h"

class StartStrategyResetIntentImpl4VIVO : public IStartStrategy{
public:
    long delayTimeMills();
    bool start(jobject context, Intent& intent);
    const char* getName();

    ~StartStrategyResetIntentImpl4VIVO();
};


#endif //BGACTIVITY_STARTSTRATEGYRESETINTENTIMPL4VIVO_H
