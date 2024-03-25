#include "AlarmManager.h"
#include "../util/Logger.h"
#include "../util/PthreadJNIEnvHelper.h"

bool AlarmManager::inited = false;
jclass AlarmManager::clsZ = nullptr;

void AlarmManager::init() {
    if (!inited) {
        LOGV("AlarmManager::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/app/AlarmManager");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}


AlarmManager::AlarmManager(const jobject &core) {
    if (!inited) {
        init();
    }
    LOGV("AlarmManager::AlarmManager, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

void AlarmManager::set(jint type, jlong triggerAtMillis, const jobject &operation) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();
    if(jenv!= nullptr && core!= nullptr && operation!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "set", "(IJLandroid/app/PendingIntent;)V");
        jenv ->CallVoidMethod(core, method, type, triggerAtMillis, operation);
    }
}

void AlarmManager::setExact(jint type, jlong triggerAtMillis, const jobject &operation) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();
    if(jenv!= nullptr && core!= nullptr && operation!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "setExact", "(IJLandroid/app/PendingIntent;)V");
        jenv ->CallVoidMethod(core, method, type, triggerAtMillis, operation);
    }
}