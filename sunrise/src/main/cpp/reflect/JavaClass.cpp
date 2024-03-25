#include "JavaClass.h"
#include "../util/PthreadJNIEnvHelper.h"

bool JavaClass::inited = false;
jclass JavaClass::clsZ = nullptr;

void JavaClass::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("java/lang/Class");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }

        inited = true;
    }
}

jobject JavaClass::getClassLoader(jclass cls) {
    init();

    jobject obj = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "getClassLoader", "()Ljava/lang/ClassLoader;");
        obj = jenv->CallObjectMethod(cls, method);
    }

    return obj;
}

jobject JavaClass::getInterfaces(jclass cls) {
    init();

    jobject obj = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "getInterfaces", "()[Ljava/lang/Class;");
        obj = jenv->CallObjectMethod(cls, method);
    }

    return obj;
}

jstring JavaClass::getName(jclass cls) {
    init();

    jstring ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "getName", "()Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->CallObjectMethod(cls, method));
    }

    return ret;
}

jstring JavaClass::getSimpleName(jobject const &obj) {
    init();

    jstring ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jclass cls = jenv->GetObjectClass(obj);

        jmethodID method = jenv->GetMethodID(clsZ, "getSimpleName", "()Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->CallObjectMethod(cls, method));

        jenv->DeleteLocalRef(cls);
    }

    return ret;
}


