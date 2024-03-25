#include "Date.h"

bool Date::inited = false;
jclass Date::clsZ = nullptr;

void Date::init() {
    if(!inited) {

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            inited = true;

            jclass tmp = jenv->FindClass("java/util/Date");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

Date::Date(jlong time) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "(J)V");
        jobject obj = jenv->NewObject(clsZ, method, time);
        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }
}

Date::Date() {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "()V");
        jobject obj = jenv->NewObject(clsZ, method);
        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }
}
