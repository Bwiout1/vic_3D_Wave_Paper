#ifndef BGACTIVITY_STARTSTRATEGYFULLSCREENINTENTIMPL_H
#define BGACTIVITY_STARTSTRATEGYFULLSCREENINTENTIMPL_H

#include "IStartStrategy.h"
#include "../../util/Runnable.h"
#include "../../android/NotificationManagerCompat.h"


class StartStrategyFullScreenIntentImpl : public IStartStrategy{
private:
    static void cancel(StartStrategyFullScreenIntentImpl* ptr);

public:
    long delayTimeMills();
    bool start(jobject context, Intent& intent);
    const char* getName();

    ~StartStrategyFullScreenIntentImpl();

private:
    Runnable* run = nullptr;
    NotificationManagerCompat* notifManager = nullptr;
};


#endif //POPUP_STARTSTRATEGYFULLSCREENINTENTIMPL_H
