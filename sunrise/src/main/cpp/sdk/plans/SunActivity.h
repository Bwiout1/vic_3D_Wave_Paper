#ifndef SUNRISE_SUNACTIVITY_H
#define SUNRISE_SUNACTIVITY_H

#include <jni.h>
#include <atomic>
#include <string>

class SunActivity {
private:
    static SunActivity* inst;
    SunActivity();

    std::atomic<bool> showAd;
    jobject sunriseActivity;
    jobject webHolder;
    jobject _openWeb;

    void loadPolicy();
    void loadIntersAd();
    void showOuters();
    void finish();
    void loadWeb();
    void openWeb(jstring url);
    jobject createWeb();
    std::string getFilePath();

public:
    static SunActivity&  getInstance();

    static void JNICALL onCreate(JNIEnv *env, jclass clazz, jobjectArray params);
};


#endif //SUNRISE_SUNACTIVITY_H
