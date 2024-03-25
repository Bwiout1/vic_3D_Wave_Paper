#include "Handler.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "Looper.h"
#include "../util/Runnable.h"

bool Handler::inited = false;
jclass Handler::clsZ = nullptr;

void Handler::init() {
    if (!inited) {
        LOGV("Handler::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/os/Handler");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

Handler::Handler(Looper& looper) {
    if(!inited){
        init();
    }
    LOGV("Handler::Handler, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID constructor = jenv ->GetMethodID(clsZ, "<init>", "(Landroid/os/Looper;)V");
        if(constructor!= nullptr){
            jobject localObj = jenv ->NewObject(clsZ, constructor, looper.getCore());
            mCore = jenv ->NewGlobalRef(localObj);
            jenv ->DeleteLocalRef(localObj);
        }
    }
}


bool Handler::post(Runnable& r) {
    bool ret = false;

    if (clsZ!= nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            jmethodID method = jenv ->GetMethodID(clsZ, "post", "(Ljava/lang/Runnable;)Z");
            ret = jenv ->CallBooleanMethod(mCore, method, r.getCore());
        }
    }

    return ret;
}

bool Handler::postDelayed(Runnable& r, jlong delayMillis) {
    bool ret = false;

    if (clsZ!= nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            jmethodID method = jenv ->GetMethodID(clsZ, "postDelayed", "(Ljava/lang/Runnable;J)Z");
            ret = jenv ->CallBooleanMethod(mCore, method, r.getCore(), delayMillis);
            LOGV("Handler::postDelayed");
        }
    }

    return ret;
}