#ifndef AND_PACKAGEMANAGER_H
#define AND_PACKAGEMANAGER_H


#include "AndroidClassWrapperBase.h"

class PackageManager  : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;
    static void init();

    const int PERMISSION_GRANTED = 0;

public:
    static jclass getClass();
    const static jint COMPONENT_ENABLED_STATE_ENABLED = 1;
    const static jint COMPONENT_ENABLED_STATE_DISABLED = 2;
    const static jint DONT_KILL_APP = 1;


public:
    PackageManager(const jobject &core);

    PackageManager(const PackageManager&)= default;
    PackageManager& operator=(const PackageManager&)=delete;

    jobject getPackageInfo(jstring packageName, jint flags);
    jint getComponentEnabledSetting(const jobject& componentName);
    void setComponentEnabledSetting(const jobject& componentName, jint newState, jint flags);
    jobject queryIntentActivities(const jobject& obj, jint num);

    jobject applicationInfo(const jobject& packageInfo);
    jobject getApplicationLabel(const jobject& applicationInfo);
    jboolean checkPermission(const char* permName, jstring packageName);

private:
    PackageManager(){};
};


#endif //AND_PACKAGEMANAGER_H
