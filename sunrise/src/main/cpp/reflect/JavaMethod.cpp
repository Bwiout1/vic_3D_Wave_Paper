#include "JavaMethod.h"
#include "../util/PthreadJNIEnvHelper.h"

bool JavaMethod::inited = false;
jclass JavaMethod::clsZ = nullptr;

void JavaMethod::init() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jclass tmp = jenv->FindClass("java/lang/reflect/Method");
        clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
        jenv ->DeleteLocalRef(tmp);
    }

    inited = true;
}

jstring JavaMethod::getName(const jobject & method) {
    if(!inited){
        init();
    }

    jstring name = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID mid = jenv->GetMethodID(clsZ, "getName", "()Ljava/lang/String;");
        name = static_cast<jstring>(jenv->CallObjectMethod(method, mid));
    }

    return name;
}
