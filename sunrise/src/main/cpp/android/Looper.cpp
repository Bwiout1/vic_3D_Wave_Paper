#include "Looper.h"
#include "../util/PthreadJNIEnvHelper.h"

bool Looper::inited = false;
jclass Looper::clsZ = nullptr;
Looper* Looper::mainLooper = nullptr;

void Looper::init() {
    if (!inited) {
        LOGV("Looper::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/os/Looper");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

Looper& Looper::getMainLooper() {
    if(!inited){
        init();
    }

    if(mainLooper== nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            jmethodID method = jenv ->GetStaticMethodID(clsZ, "getMainLooper", "()Landroid/os/Looper;");
            jobject localObj = jenv ->CallStaticObjectMethod(clsZ, method);

            mainLooper = new Looper(localObj);
            jenv->DeleteLocalRef(localObj);
        }
    }

    return *mainLooper;
}

Looper::Looper(const jobject &core) {
    if (!inited) {
        init();
    }
    LOGV("Looper::Looper, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

