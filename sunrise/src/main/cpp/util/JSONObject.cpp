#include "JSONObject.h"

bool JSONObject::inited = false;
jclass JSONObject::clsZ = nullptr;

void JSONObject::init() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        inited = true;

        jclass tmp = jenv->FindClass("org/json/JSONObject");
        clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
        jenv ->DeleteLocalRef(tmp);
    }
}

JSONObject::JSONObject(jstring core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "(Ljava/lang/String;)V");
        jobject obj = jenv->NewObject(clsZ, method, core);
        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }
}

jboolean JSONObject::has(const char *str) {
    jboolean ret = false;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jstring jstr = jenv->NewStringUTF(str);

        jmethodID method = jenv->GetMethodID(clsZ, "has", "(Ljava/lang/String;)Z");
        ret = jenv->CallBooleanMethod(mCore, method, jstr);

        jenv->DeleteLocalRef(jstr);
    }
    return ret;
}

jstring JSONObject::getString(const char* key) {
    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jstring jstr = jenv->NewStringUTF(key);

        jmethodID method = jenv->GetMethodID(clsZ, "getString", "(Ljava/lang/String;)Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->CallObjectMethod(mCore, method, jstr));

        jenv->DeleteLocalRef(jstr);
    }
    return ret;
}
