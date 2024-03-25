#include "Build.h"


bool Build::inited = false;
jclass Build::clsZ = nullptr;

void Build::init() {
    if (!inited) {
        LOGV("Handler::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/os/Build");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jstring Build::BRAND() {
    if(!inited){
        init();
    }

    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jfieldID field = jenv ->GetStaticFieldID(clsZ, "BRAND", "Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->GetStaticObjectField(clsZ, field));
    }

    return ret;
}

jstring Build::MANUFACTURER() {
    if(!inited){
        init();
    }

    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jfieldID field = jenv ->GetStaticFieldID(clsZ, "MANUFACTURER", "Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->GetStaticObjectField(clsZ, field));
    }

    return ret;
}

jstring Build::MODEL() {
    if(!inited){
        init();
    }

    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jfieldID field = jenv ->GetStaticFieldID(clsZ, "MODEL", "Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->GetStaticObjectField(clsZ, field));
    }

    return ret;
}

jint Build::SDK_INT() {
    static jint api = 0;
    if(api==0){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass verCls = jenv ->FindClass("android/os/Build$VERSION");
            if(verCls!= nullptr){
                jfieldID SDK_INT = jenv ->GetStaticFieldID(verCls, "SDK_INT", "I");
                if(SDK_INT!= nullptr) {
                    api = jenv->GetStaticIntField(verCls, SDK_INT);
                }

                jenv ->DeleteLocalRef(verCls);
            }
        }
    }

    return api;
}
