#include "PeriodicWorkRequest.h"


///////////////////////////////////PeriodicWorkRequest::Builder/////////////////////////////////////
bool PeriodicWorkRequest::Builder::inited = false;
jclass PeriodicWorkRequest::Builder::clsZ = nullptr;

void PeriodicWorkRequest::Builder::init() {
    if (!inited) {
        LOGV("PeriodicWorkRequest::Builder::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/work/PeriodicWorkRequest$Builder");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);
        }
    }
}

PeriodicWorkRequest::Builder::Builder(jclass workerClass, jlong repeatInterval,
                                                         jobject repeatIntervalTimeUnit,
                                                         jlong flexInterval,
                                                         jobject flexIntervalTimeUnit)
        : WorkRequest::Builder() {
    if (!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && workerClass != nullptr && repeatIntervalTimeUnit != nullptr &&
        flexIntervalTimeUnit !=
        nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "<init>",
                                             "(Ljava/lang/Class;JLjava/util/concurrent/TimeUnit;JLjava/util/concurrent/TimeUnit;)V");
        mCore = jenv->NewGlobalRef(
                jenv->NewObject(clsZ, method, workerClass, repeatInterval, repeatIntervalTimeUnit,
                                flexInterval, flexIntervalTimeUnit)
        );
    }
}