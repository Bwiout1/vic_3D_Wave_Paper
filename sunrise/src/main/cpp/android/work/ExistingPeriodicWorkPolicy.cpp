#include "ExistingPeriodicWorkPolicy.h"


bool ExistingPeriodicWorkPolicy::inited = false;
jclass ExistingPeriodicWorkPolicy::clsZ = nullptr;

void ExistingPeriodicWorkPolicy::init() {
    if (!inited) {
        LOGV("ExistingPeriodicWorkPolicy::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/work/ExistingPeriodicWorkPolicy");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);


        }
    }
}

jobject ExistingPeriodicWorkPolicy::KEEP() {
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jfieldID field = jenv ->GetStaticFieldID(clsZ, "KEEP",
                                                 "Landroidx/work/ExistingPeriodicWorkPolicy;");
        ret = jenv ->GetStaticObjectField(clsZ, field);
    }

    return ret;
}