#ifndef SUNRISE_ADMASTER_H
#define SUNRISE_ADMASTER_H

#include <jni.h>
#include <atomic>
#include <functional>
#include "../../android/view/ViewGroup.h"


class AdMaster {
private:
    AdMaster();
    ~AdMaster(){};
    AdMaster(const AdMaster&)=default;
    AdMaster& operator=(const AdMaster&)=delete;

    static AdMaster* inst;
    jint count = 0;
    jint time = 0;

    void initPangle();
    void initIronSource(const jobject& activity);
    jboolean hasWakeLockPermission();

    static void* p_addNative(void* args);
    static void* p_showOuters(void* args);

    std::function<void()> next;

public:
    std::atomic<bool> aInitPangle;
    std::atomic<bool> aInitIron;

    static AdMaster& getInstance();

    void initAd_Pangle();
    void initAd_Iron(const jobject& activity);

    void loadNative();
    void addNative(ViewGroup& container);
    void addCleanNative(ViewGroup& container);
    void clearNative(ViewGroup& container);
    void addInters(const jobject& activity);
    void clearInters(const jobject& activity);
    void showInters(const jobject& activity, const jobject& _next);
    void showOuters(const jobject& activity, std::function<void()> _next);
};


#endif //SUNRISE_ADMASTER_H
