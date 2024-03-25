#include "IronSourceError.h"

bool IronSourceError::inited = false;
jclass IronSourceError::clsZ = nullptr;

void IronSourceError::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/ironsource/mediationsdk/logger/IronSourceError");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

IronSourceError::IronSourceError(jobject const &core) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

jstring IronSourceError::toString() {
    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "toString", "()Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->CallObjectMethod(getCore(), method));
    }

    return ret;
}

jint IronSourceError::getErrorCode() {
    jint ret = -1;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "getErrorCode", "()I");
        ret = jenv->CallIntMethod(getCore(), method);
    }

    return ret;
}
