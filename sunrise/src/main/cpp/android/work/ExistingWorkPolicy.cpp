#include "ExistingWorkPolicy.h"


bool ExistingWorkPolicy::inited = false;
jclass ExistingWorkPolicy::clsZ = nullptr;

void ExistingWorkPolicy::init() {
    if (!inited) {
        LOGV("ExistingWorkPolicy::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/work/ExistingWorkPolicy");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);


        }
    }
}

jobject ExistingWorkPolicy::APPEND_OR_REPLACE() {
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jfieldID field = jenv ->GetStaticFieldID(clsZ, "APPEND_OR_REPLACE", "Landroidx/work/ExistingWorkPolicy;");
        ret = jenv ->GetStaticObjectField(clsZ, field);
    }

    return ret;
}