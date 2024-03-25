#ifndef CONFIG_FLURRYCONFIG_H
#define CONFIG_FLURRYCONFIG_H


#include "../android/AndroidClassWrapperBase.h"

class FlurryConfig : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static FlurryConfig* inst;
    static void init();

public:
    static FlurryConfig& getInstance();

public:
    void fetchConfig();
    jboolean activateConfig();

    void registerListener(jobject listener);
    void registerListener(jobject listener, jobject handler);
    void unregisterListener(jobject listener);

    jlong getLong(const char* key, jlong defVal);
    jstring getString(const char* key, jstring defVal);
    jboolean getBoolean(jstring key, jboolean defVal);
    jint getInt(const char* key, jint defVal);

private:
    FlurryConfig(const jobject& core);

    FlurryConfig(){}
    FlurryConfig(const FlurryConfig&)= delete;
    FlurryConfig& operator=(const FlurryConfig&)=delete;
};


#endif //CONFIG_FLURRYCONFIG_H
