#include "Resources.h"

bool Resources::inited = false;
jclass Resources::clsZ = nullptr;

void Resources::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/content/res/Resources");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

Resources::Resources(const jobject &core) {
    if(!inited){
        init();
    }

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        mCore = jenv->NewGlobalRef(core);
    }
}


jobject Resources::openRawResource(jint resId){
    jobject ret = nullptr;

    if(clsZ!= nullptr && mCore != nullptr) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID method = jenv ->GetMethodID(clsZ, "openRawResource", "(I)Ljava/io/InputStream;");
            if(method != nullptr){
                ret = jenv ->CallObjectMethod(getCore(), method, resId);
            }
        }
    }

    return ret;
}

jint Resources::getIdentifier(jstring name, jstring type, jstring pkg) {
    jint id = 0;

    if(clsZ!= nullptr && mCore != nullptr) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID method = jenv ->GetMethodID(clsZ, "getIdentifier",
                                                  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
            if(method!= nullptr){
                id = jenv ->CallIntMethod(getCore(), method, name, type, pkg);
            }
        }
    }

    return id;
}

jobject Resources::getConfiguration() {
    jobject ret = nullptr;

    if(clsZ!= nullptr && mCore != nullptr) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID method = jenv ->GetMethodID(clsZ, "getConfiguration", "()Landroid/content/res/Configuration;");
            if(method != nullptr){
                ret = jenv ->CallObjectMethod(getCore(), method);
            }
        }
    }

    return ret;
}
