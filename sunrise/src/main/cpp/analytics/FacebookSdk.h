#ifndef SUNRISE_FACEBOOKSDK_H
#define SUNRISE_FACEBOOKSDK_H

#include <jni.h>
#include "../android/Context.h"


class FacebookSdk {
private:
    static bool inited;
    static jclass clsZ;

    static void init();
public:
    static jboolean isInitialized();
    static void sdkInitialize(Context& context);
};


#endif //SUNRISE_FACEBOOKSDK_H
