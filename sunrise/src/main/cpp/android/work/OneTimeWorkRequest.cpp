#include "OneTimeWorkRequest.h"

/////////////////////////////////////////OneTimeWorkRequest::Builder////////////////////////////////
bool OneTimeWorkRequest::Builder::inited = false;
jclass OneTimeWorkRequest::Builder::clsZ = nullptr;

void OneTimeWorkRequest::Builder::init() {
    if (!inited) {
        LOGV("OneTimeWorkRequest::Builder::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/work/OneTimeWorkRequest$Builder");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

OneTimeWorkRequest::Builder::Builder(jclass workerClass) : WorkRequest::Builder(){
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && workerClass!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "<init>", "(Ljava/lang/Class;)V");
        mCore = jenv ->NewGlobalRef(
                jenv ->NewObject(clsZ, method, workerClass)
                );
    }
}