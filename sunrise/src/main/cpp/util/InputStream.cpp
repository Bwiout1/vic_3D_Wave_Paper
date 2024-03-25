#include "InputStream.h"
#include "../util/Logger.h"
#include "../util/PthreadJNIEnvHelper.h"

bool InputStream::inited = false;
jclass InputStream::clsZ = nullptr;

void InputStream::init() {
    if (!inited) {
        LOGV("InputStream::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/io/InputStream");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

InputStream::InputStream(const jobject &core) {
    if(!inited){
        init();
    }

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        mCore = jenv->NewGlobalRef(core);
    }
}


jint InputStream::available(){
    if(clsZ!= nullptr && mCore != nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID method = jenv ->GetMethodID(clsZ, "available","()I");
            if(method!=nullptr) {
                return jenv->CallIntMethod(getCore(), method);
            }
        }
    }

    return 0;
}

jlong InputStream::skip(jlong n) {
    if(clsZ!= nullptr && mCore != nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID method = jenv ->GetMethodID(clsZ, "skip","(J)J");
            if(method!= nullptr){
                return jenv ->CallLongMethod(getCore(), method, n);
            }
        }
    }

    return 0;
}

jint InputStream::read(jbyteArray buf) {
    if(clsZ!= nullptr && mCore != nullptr) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID  method = jenv ->GetMethodID(clsZ, "read", "([B)I");
            if(method!= nullptr){
                return jenv ->CallIntMethod(getCore(), method, buf);
            }
        }
    }

    return -1;
}

void InputStream::close() {
    if(clsZ!= nullptr && mCore != nullptr) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID close = jenv ->GetMethodID(clsZ, "close", "()V");
            if(close!= nullptr){
                return jenv ->CallVoidMethod(getCore(), close);
            }
        }
    }
}
