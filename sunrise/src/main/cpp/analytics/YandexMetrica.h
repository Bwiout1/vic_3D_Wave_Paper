#ifndef SUNRISE_YANDEXMETRICA_H
#define SUNRISE_YANDEXMETRICA_H

#include "../android/AndroidClassWrapperBase.h"
#include "../util/HashMap.h"


class YandexMetrica: public AndroidClassWrapperBase  {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static void activate(const jobject& app, const jobject& config);
    static void enableActivityAutoTracking(const jobject& app);

    static void reportEvent(jstring jstr);
    static void reportEvent(jstring jstr, HashMap& map);

};


#endif //SUNRISE_YANDEXMETRICA_H
