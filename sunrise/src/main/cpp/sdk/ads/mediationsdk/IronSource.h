#ifndef SUNRISE_IRONSOURCE_H
#define SUNRISE_IRONSOURCE_H


#include "../../../android/AndroidClassWrapperBase.h"


class IronSource: public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void _init();

public:
    static void removeInterstitialListener();
    static void setInterstitialListener(const jobject& listener);
    static void loadInterstitial();
    static void showInterstitial();
    static jstring getAdvertiserId(const jobject& activity);
    static void setUserId(jstring jstr);
    static void setMetaData(const char* str1, const char* str2);
    static void init(const jobject& activity, jstring jstr);
    static void shouldTrackNetworkState(const jobject& activity, jboolean flag);
    static void setAdaptersDebug(jboolean flag);

    static void onResume(const jobject& activity);
    static void onPause(const jobject& activity);
};


#endif //SUNRISE_IRONSOURCE_H
