#ifndef POPUP_STARTSTRATEGYVIRTUALDISPLAYIMPL_H
#define POPUP_STARTSTRATEGYVIRTUALDISPLAYIMPL_H


#include "IStartStrategy.h"

class Runnable;
class PendingIntent;

class StartStrategyVirtualDisplayImpl: public IStartStrategy {
private:
    static void send(StartStrategyVirtualDisplayImpl* virtualDisplayImpl);

public:
    long delayTimeMills();
    bool start(jobject context, Intent& intent);
    const char* getName();

    ~StartStrategyVirtualDisplayImpl();
private:
    Runnable* run= nullptr;
    PendingIntent* pendingIntent= nullptr;
};


#endif //POPUP_STARTSTRATEGYVIRTUALDISPLAYIMPL_H
