#include "AndroidClassWrapperBase.h"

bool AndroidClassWrapperBase::inited = false;
jclass AndroidClassWrapperBase::clsZ = nullptr;

void AndroidClassWrapperBase::init() {
    if (!inited) {
        LOGV("AlarmManager::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/lang/Object");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jobject AndroidClassWrapperBase::getCore(){
    return mCore ;
}

jint AndroidClassWrapperBase::hashCode() {
    if (!inited) {
        init();
    }

    jint ret = 0;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "hashCode",
                                             "()I");

        ret = jenv->CallIntMethod(mCore, method);
    }

    return ret;
}