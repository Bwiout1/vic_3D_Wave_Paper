#include "System.h"


bool System::inited = false;
jclass System::clsZ = nullptr;

void System::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/lang/System");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jlong System::currentTimeMillis() {
    if(!inited){
        init();
    }

    jlong ret = 0;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv ->GetStaticMethodID(clsZ, "currentTimeMillis", "()J");
        ret = jenv ->CallStaticLongMethod(clsZ, method);
    }

    return ret;
}