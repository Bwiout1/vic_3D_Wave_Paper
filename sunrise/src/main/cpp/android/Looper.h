#ifndef ANDROID_LOOPER_H
#define ANDROID_LOOPER_H

#include "../util/Logger.h"
#include "AndroidClassWrapperBase.h"

class Looper : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static Looper* mainLooper;

    static void init();

public:
    static Looper& getMainLooper();

public:
    Looper(const Looper&)=delete;
    Looper& operator=(const Looper&)=delete;

private:
    Looper(){}

    Looper(const jobject& core);
};


#endif //ANDROID_LOOPER_H
