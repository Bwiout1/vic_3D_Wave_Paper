#include "WorkRequest.h"



/////////////////////////////////////////WorkRequest::Builder///////////////////////////////////////
bool WorkRequest::Builder::inited = false;
jclass WorkRequest::Builder::clsZ = nullptr;

void WorkRequest::Builder::init() {
    if (!inited) {
        LOGV("WorkRequest::Builder::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/work/WorkRequest$Builder");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

WorkRequest::Builder& WorkRequest::Builder::setInitialDelay(jlong duration, const jobject timeUnit) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && timeUnit!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ,
                                              "setInitialDelay",
                                              "(JLjava/util/concurrent/TimeUnit;)Landroidx/work/WorkRequest$Builder;")   ;
        jenv ->CallObjectMethod(mCore, method, duration, timeUnit);
    }

    return  *this;
}

jobject WorkRequest::Builder::build() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ,
                                              "build",
                                              "()Landroidx/work/WorkRequest;");
        ret = jenv ->CallObjectMethod(mCore, method);
    }

    return ret;
}