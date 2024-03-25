#include "FlurryConfig.h"

bool FlurryConfig::inited = false;
jclass FlurryConfig::clsZ = nullptr;
FlurryConfig* FlurryConfig::inst = nullptr;

void FlurryConfig::init() {
    if(!inited) {
        LOGV("FlurryConfig::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            inited = true;

            jclass tmp = jenv->FindClass("com/flurry/android/FlurryConfig");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

FlurryConfig& FlurryConfig::getInstance() {
    if(!inited){
        init();
    }

    if(inst == nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            jmethodID method = jenv ->GetStaticMethodID(clsZ, "getInstance", "()Lcom/flurry/android/FlurryConfig;");

            inst = new FlurryConfig(jenv ->CallStaticObjectMethod(clsZ, method));
        }
    }

    return *inst;
}

FlurryConfig::FlurryConfig(const jobject &core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

void FlurryConfig::fetchConfig() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "fetchConfig", "()V");
        jenv ->CallVoidMethod(mCore, method);
    }
}

jboolean FlurryConfig::activateConfig() {
    jboolean ret = false;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "activateConfig", "()Z");
        ret = jenv ->CallBooleanMethod(mCore, method);
    }

    return ret;
}

jlong FlurryConfig::getLong(const char* key, jlong defVal) {
    jlong ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore!= nullptr){
        jstring jstr = jenv->NewStringUTF(key);
        jmethodID method = jenv ->GetMethodID(clsZ, "getLong", "(Ljava/lang/String;J)J");
        ret = jenv ->CallLongMethod(mCore, method, jstr, defVal) ;

        jenv->DeleteLocalRef(jstr);
    }

    return ret ;
}

jstring FlurryConfig::getString(const char* key, jstring defVal) {
    jstring ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore!= nullptr){
        jstring jstr = jenv->NewStringUTF(key);
        jmethodID method = jenv ->GetMethodID(clsZ, "getString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->CallObjectMethod(mCore, method, jstr, defVal));

        jenv->DeleteLocalRef(jstr);
    }

    return ret ;
}

jboolean FlurryConfig::getBoolean(jstring key, jboolean defVal) {
    jboolean ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "getBoolean", "(Ljava/lang/String;Z)Z");
        ret = jenv->CallBooleanMethod(mCore, method, key, defVal);
    }

    return ret ;
}

jint FlurryConfig::getInt(const char *key, jint defVal) {
    jint ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore!= nullptr){
        jstring jstr = jenv->NewStringUTF(key);
        jmethodID method = jenv ->GetMethodID(clsZ, "getInt", "(Ljava/lang/String;I)I");
        ret = jenv ->CallIntMethod(mCore, method, jstr, defVal) ;

        jenv->DeleteLocalRef(jstr);
    }

    return ret ;
}

void FlurryConfig::registerListener(jobject listener) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    if (jenv != nullptr && mCore!= nullptr && listener!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "registerListener",
                                             "(Lcom/flurry/android/FlurryConfigListener;)V");

        jenv->CallVoidMethod(mCore, method, listener);
    }
}

void FlurryConfig::registerListener(jobject listener, jobject handler){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    if(jenv!= nullptr && mCore!= nullptr && listener!= nullptr && handler!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ,
                                              "registerListener",
                                              "(Lcom/flurry/android/FlurryConfigListener;Landroid/os/Handler;)V");
        jenv ->CallVoidMethod(mCore, method, listener, handler);
    }
}

void FlurryConfig::unregisterListener (jobject listener) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    if (jenv != nullptr && mCore!= nullptr && listener!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "unregisterListener",
                                             "(Lcom/flurry/android/FlurryConfigListener;)V");

        jenv->CallVoidMethod(mCore, method, listener);
    }
}
