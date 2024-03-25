#ifndef SUNRISE_TTADSDK_H
#define SUNRISE_TTADSDK_H

#include "../../../android/AndroidClassWrapperBase.h"
class TTAdNative;


class TTAdSdk : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void _init();

public:
    TTAdSdk();
    TTAdSdk& getAdManager();
    TTAdNative* createAdNative();

    static void init(const jobject& context, const jobject& ttAdConfig, const jobject& initCallBack);
    static jboolean isInitSuccess();
};


#endif //SUNRISE_TTADSDK_H
