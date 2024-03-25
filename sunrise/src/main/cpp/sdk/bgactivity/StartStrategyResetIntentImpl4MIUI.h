#ifndef BGACTIVITY_STARTSTRATEGYRESETINTENTIMPL4MIUI_H
#define BGACTIVITY_STARTSTRATEGYRESETINTENTIMPL4MIUI_H

#include "IStartStrategy.h"

class StartStrategyResetIntentImpl4MIUI : public IStartStrategy{
public:
    long delayTimeMills();
    bool start(jobject context, Intent& intent);
    const char* getName();

    ~StartStrategyResetIntentImpl4MIUI();
};


#endif //BGACTIVITY_STARTSTRATEGYRESETINTENTIMPL4MIUI_H
