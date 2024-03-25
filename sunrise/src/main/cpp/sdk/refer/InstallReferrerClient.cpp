#include "InstallReferrerClient.h"
#include "../../android/Application.h"
#include "InstallReferrerStateListener.h"

bool InstallReferrerClient::inited = false;
jclass InstallReferrerClient::clsZ = nullptr;

void InstallReferrerClient::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("com/android/installreferrer/api/InstallReferrerClient");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            inited = true;
        }
    }
}

jobject InstallReferrerClient::newBuilder(jobject const &context) {
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && context!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "newBuilder", "(Landroid/content/Context;)Lcom/android/installreferrer/api/InstallReferrerClient$Builder;");
        ret = jenv->CallStaticObjectMethod(clsZ, method, context);
    }

    return ret;
}

InstallReferrerClient::InstallReferrerClient(jobject const &core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core!= nullptr){
        mCore = jenv ->NewGlobalRef(core) ;
    }
}

void InstallReferrerClient::startConnection(const jobject& listener) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "startConnection", "(Lcom/android/installreferrer/api/InstallReferrerStateListener;)V");
        jenv->CallVoidMethod(mCore, method, listener);
    }
}

jstring InstallReferrerClient::installReferrer() {
    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method1 = jenv->GetMethodID(clsZ, "getInstallReferrer", "()Lcom/android/installreferrer/api/ReferrerDetails;");
        jobject obj = jenv->CallObjectMethod(mCore, method1);

        jclass cls = jenv->FindClass("com/android/installreferrer/api/ReferrerDetails");
        jmethodID method2 = jenv->GetMethodID(cls, "getInstallReferrer", "()Ljava/lang/String;");

        ret = static_cast<jstring>(jenv->CallObjectMethod(obj, method2));

        jenv->DeleteLocalRef(obj);
        jenv->DeleteLocalRef(cls);
    }

    return ret;
}

void InstallReferrerClient::endConnection() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "endConnection", "()V");
        jenv->CallVoidMethod(mCore, method);
    }
}


////////////////////////////InstallReferrerClient::Builder//////////////////////////////////////////
bool InstallReferrerClient::Builder::inited = false;
jclass InstallReferrerClient::Builder::clsZ = nullptr;

void InstallReferrerClient::Builder::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("com/android/installreferrer/api/InstallReferrerClient$Builder");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);

            inited = true;
        }
    }
}

InstallReferrerClient::Builder::Builder(jobject const &core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core!= nullptr){
        mCore = jenv ->NewGlobalRef(core) ;
    }
}

jobject InstallReferrerClient::Builder::build() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "build", "()Lcom/android/installreferrer/api/InstallReferrerClient;");
        ret = jenv ->CallObjectMethod(mCore, method);
    }

    return ret;
}
