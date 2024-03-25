#include "TimeUnit.h"

bool TimeUnit::inited = false;
jclass TimeUnit::clsZ = nullptr;

void TimeUnit::init() {
    if (!inited) {
        LOGV("TimeUnit::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/util/concurrent/TimeUnit");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);


        }
    }
}

jobject TimeUnit::SECONDS(){
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && clsZ != nullptr) {
        jfieldID field = jenv->GetStaticFieldID(clsZ, "SECONDS", "Ljava/util/concurrent/TimeUnit;");
        ret = jenv->GetStaticObjectField(clsZ, field);
    }

    return ret;
}

jobject TimeUnit::MINUTES(){
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && clsZ != nullptr) {
        jfieldID field = jenv->GetStaticFieldID(clsZ, "MINUTES", "Ljava/util/concurrent/TimeUnit;");
        ret = jenv->GetStaticObjectField(clsZ, field);
    }

    return ret;
}