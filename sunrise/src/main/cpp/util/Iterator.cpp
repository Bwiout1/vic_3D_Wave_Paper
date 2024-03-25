#include "Iterator.h"


bool Iterator::inited = false;
jclass Iterator::clsZ = nullptr;

void Iterator::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/util/Iterator");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

Iterator::Iterator(const jobject& core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

jboolean Iterator::hasNext() {
    jboolean ret = false;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "hasNext", "()Z");
        ret = jenv ->CallBooleanMethod(mCore, method);
    }

    return ret;
}

jobject Iterator::next() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "next", "()Ljava/lang/Object;");
        ret = jenv ->CallObjectMethod(mCore, method);
    }

    return ret;
}