#ifndef AND_CONTEXT_H
#define AND_CONTEXT_H

#include "AndroidClassWrapperBase.h"
#include "Intent.h"

class Context : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass contextClsZ;
    static jclass activityClsZ;
    static void init();

public:
    static jclass getContextClass();
    static jclass getActivityClass();

public:
    Context(const jobject &core);

    Context(const Context&)= default;
    Context& operator=(const Context&)=delete;

    void startActivity(const jobject& intent);
    jobject startForegroundService(const jobject& intent);
    jobject getSystemService(const char * name);
    jobject getPackageManager();
    jstring getPackageName();
    jobject getResources();
    jstring getProcessName();
    jstring getAppName();

private:
    Context(){}
};


#endif //AND_CONTEXT_H
