#include "ActivityInfo.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"

bool ActivityInfo::inited = false;
jclass ActivityInfo::clsZ = nullptr;

void ActivityInfo::init() {
    if (!inited) {
        LOGV("ActivityInfo::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/content/pm/ActivityInfo");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

ActivityInfo::ActivityInfo(const jobject &core) {
    if (!inited) {
        init();
    }
    LOGV("ActivityInfo::ActivityInfo, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);

        jfieldID fid = jenv ->GetFieldID(clsZ, "name", "Ljava/lang/String;");
        name = static_cast<jstring>(jenv->NewGlobalRef(jenv->GetObjectField(getCore(), fid)));

        fid = jenv ->GetFieldID(clsZ, "targetActivity", "Ljava/lang/String;");
        targetActivity = static_cast<jstring>(jenv->NewGlobalRef(
                jenv->GetObjectField(getCore(), fid)));

        fid = jenv->GetFieldID(clsZ, "packageName", "Ljava/lang/String;");
        packageName = static_cast<jstring>(jenv->NewGlobalRef(
                jenv->GetObjectField(getCore(), fid)));
    }
}

jstring ActivityInfo::loadLabel(const jobject& pm) {
    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();
    if(jenv!= nullptr && core!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "loadLabel",
                                             "(Landroid/content/pm/PackageManager;)Ljava/lang/CharSequence;");
        ret = static_cast<jstring>(jenv->CallObjectMethod(core, method, pm));
    }
    return ret;
}

jint ActivityInfo::getIconResource() {
    jint ret = -1;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();
    if(jenv!= nullptr && core!= nullptr)  {
        jmethodID method = jenv->GetMethodID(clsZ, "getIconResource", "()I");
        ret = jenv->CallIntMethod(core, method);
    }

    return ret;
}

ActivityInfo::~ActivityInfo() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv== nullptr)
        return;

    if(name!= nullptr){
        jenv ->DeleteGlobalRef(name);
        name = nullptr;
    }

    if(targetActivity!= nullptr){
        jenv ->DeleteGlobalRef(targetActivity);
        targetActivity = nullptr;
    }

    if(packageName != nullptr){
        jenv ->DeleteGlobalRef(packageName);
        packageName = nullptr;
    }
}
