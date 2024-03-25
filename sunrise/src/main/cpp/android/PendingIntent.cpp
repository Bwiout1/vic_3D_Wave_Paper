#include "PendingIntent.h"
#include "../util/Logger.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/JniException.h"

bool PendingIntent::inited = false;
jclass PendingIntent::clsZ = nullptr;

void PendingIntent::init() {
    if (!inited) {
        LOGV("PendingIntent::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/app/PendingIntent");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jobject PendingIntent::getActivity(const jobject& context, jint requestCode, const jobject& intent,
                           jint flags) {
    if (!inited) {
        init();
    }
    LOGV("PendingIntent::getActivity");

    jobject ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && context!= nullptr && intent!= nullptr){
        jmethodID method = jenv ->GetStaticMethodID(clsZ, "getActivity", "(Landroid/content/Context;ILandroid/content/Intent;I)Landroid/app/PendingIntent;");
        ret = jenv ->CallStaticObjectMethod(clsZ, method, context, requestCode, intent, flags);
        if(check_and_clear_exception(jenv)){
            LOGE("PendingIntent::getActivity error");
        }
    }
    return ret;
}


PendingIntent::PendingIntent(const jobject& core) {
    if (!inited) {
        init();
    }
    LOGV("PendingIntent::PendingIntent, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

void PendingIntent::send() {
    LOGV("PendingIntent::PendingIntent, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();

    if(jenv!= nullptr && core!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "send", "()V");
        jenv ->CallVoidMethod(core, method);
    }
}

void PendingIntent::send(int code) {
    LOGV("PendingIntent::PendingIntent, send(int code)");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();

    if(jenv!= nullptr && core!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "send", "(I)V");
        jenv ->CallVoidMethod(core, method, code);
    }
}
