#ifndef SUNRISE_APPLIFECYCLEMANAGER_H
#define SUNRISE_APPLIFECYCLEMANAGER_H


#include "../../android/Application.h"

class AppLifecycleManager : public Application::ActivityLifecycleCallbacks {
public:
    static AppLifecycleManager* inst;

    static jclass activityArray[9];
    static void initActivityArray();
    static bool activityArray_inited;

public:
    static AppLifecycleManager& getInstance();
    AppLifecycleManager() : Application::ActivityLifecycleCallbacks(){};

    void onActivityCreated(jobject activity, jobject savedInstanceState);
    void onActivityStarted(jobject activity);
    void onActivityResumed(jobject activity);
    void onActivityPaused(jobject activity);
    void onActivityStopped(jobject activity);
    void onActivityDestroyed(jobject activity);

    bool isAppForeground();

    void registerCallback();

private:
    int foregroundCount=0;

    bool isAdAbout(const jobject& activity);
    void shadow(const jobject& _activity);
    jobject catchAdView(const jobject& _activity);
};


#endif //SUNRISE_APPLIFECYCLEMANAGER_H
