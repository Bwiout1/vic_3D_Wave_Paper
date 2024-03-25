#ifndef SUNRISE_YANDEXMETRICACONFIG_H
#define SUNRISE_YANDEXMETRICACONFIG_H

#include "../android/AndroidClassWrapperBase.h"

class YandexMetricaConfig {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    class Builder;

    static jobject newConfigBuilder(const char* _key);
};

class YandexMetricaConfig::Builder: public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    Builder(const jobject core);

    Builder& withCrashReporting(jboolean flag);
    Builder& withNativeCrashReporting(jboolean flag);

    jobject build();
};


#endif //SUNRISE_YANDEXMETRICACONFIG_H
