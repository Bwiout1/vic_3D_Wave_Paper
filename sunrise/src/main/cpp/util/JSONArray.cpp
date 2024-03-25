#include "JSONArray.h"


bool JSONArray::inited = false;
jclass JSONArray::clsZ = nullptr;

void JSONArray::init() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        inited = true;

        jclass tmp = jenv->FindClass("org/json/JSONArray");
        clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

        jenv ->DeleteLocalRef(tmp);
    }
}

JSONArray::JSONArray(jstring str) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {

        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "(Ljava/lang/String;)V");
        jobject obj = jenv->NewObject(clsZ, method, str);
        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }
}

jint JSONArray::length() {
    jint ret = 0;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "length", "()I");
        ret = jenv->CallIntMethod(mCore, method);
    }

    return ret;
}

jstring JSONArray::optString(jint num) {
    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "optString", "(I)Ljava/lang/String;");

        ret = static_cast<jstring>(jenv->CallObjectMethod(mCore, method, num));
    }

    return ret;
}
