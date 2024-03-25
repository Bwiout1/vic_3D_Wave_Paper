#include "ResolveInfo.h"


bool ResolveInfo::inited = false;
jclass ResolveInfo::clsZ = nullptr;

void ResolveInfo::init() {
    if (!inited) {
        LOGV("ResolveInfo::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/content/pm/ResolveInfo");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jobject ResolveInfo::activityInfo(const jobject &core) {
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core!= nullptr){
        jfieldID field = jenv ->GetFieldID(clsZ, "activityInfo",
                                           "Landroid/content/pm/ActivityInfo;");
        ret = jenv ->GetObjectField(core, field);
    }

    return ret;
}
