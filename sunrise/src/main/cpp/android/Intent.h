#ifndef AND_INTENT_H
#define AND_INTENT_H


#include "AndroidClassWrapperBase.h"

class Intent : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static const jint FLAG_ACTIVITY_NO_ANIMATION = 65536;
    static const jint FLAG_FROM_BACKGROUND = 4;
    static const jint FLAG_ACTIVITY_NEW_TASK = 268435456;
    static const jint FLAG_ACTIVITY_CLEAR_TOP = 67108864;
    static const jint FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS = 8388608;

public:
    Intent(const jobject& core);

    Intent();
    Intent(const jobject& packageContext, jclass cls);
    Intent(const char* action, const jobject& uri);

    Intent(const Intent&)= default;
    Intent& operator=(const Intent&)=delete;

    Intent& addCategory(const char * str);
    Intent& addFlags(jint flag);
    Intent& setPackage(jstring pkg);
    Intent& setComponent(const jobject& component);
    Intent& setAction(const char * action);
    Intent& putExtra(const char * name, const char* value);
    Intent& putExtra(const char * name, jint value);
    Intent& putExtra(const char * name, jlong value);
    Intent& putExtra(const char * name, const jobject& parcelable);
    jlong getLongExtra(const char* name, jlong defVal);
    jint getIntExtra(const char* name, jint defVal);
    jobject getParcelableExtra(const char* name);
};


#endif //AND_INTENT_H
