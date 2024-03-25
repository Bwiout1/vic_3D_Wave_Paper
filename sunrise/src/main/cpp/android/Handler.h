#ifndef ANDROID_HANDLER_H
#define ANDROID_HANDLER_H

#include "../util/Logger.h"
#include "AndroidClassWrapperBase.h"

class Looper;
class Runnable;

class Handler : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Handler(Looper& looper);
    Handler(const Handler&)= default;
    Handler& operator=(const Handler&)=delete;

    bool post(Runnable& r);
    bool postDelayed(Runnable& r, jlong delayMillis);
private:
    Handler(){}
};


#endif //ANDROID_HANDLER_H
