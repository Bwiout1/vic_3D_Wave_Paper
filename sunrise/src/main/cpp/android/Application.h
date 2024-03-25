#ifndef AND_APPLICATION_H
#define AND_APPLICATION_H


#include "Context.h"
#include "../util/Logger.h"
#include "../reflect/BaseJavaInterface.h"

class Application : public Context {
private:
    static bool inited;
    static jclass clsZ;
    static Application* inst;
    static void init();

public:
    static Application& getInstance();

public:
    Application(const Application&)= default;
    Application& operator=(const Application&)=delete;

    void registerActivityLifecycleCallbacks(jobject callback);

private:
    Application(const jobject &core);

public:
    class ActivityLifecycleCallbacks;
};

class Application::ActivityLifecycleCallbacks : public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    ActivityLifecycleCallbacks();
    jobject invoke(jobject method, jobjectArray objects);

    virtual void onActivityCreated(jobject activity, jobject savedInstanceState)=0;
    virtual void onActivityStarted(jobject activity)=0;
    virtual void onActivityResumed(jobject activity)=0;
    virtual void onActivityPaused(jobject activity)=0;
    virtual void onActivityStopped(jobject activity)=0;
    virtual void onActivityDestroyed(jobject activity)=0;

private:
    ActivityLifecycleCallbacks(const ActivityLifecycleCallbacks&)=delete;
    ActivityLifecycleCallbacks& operator=(const ActivityLifecycleCallbacks&)=delete;
};


#endif //AND_APPLICATION_H
