#include "UserInfo.h"


bool UserInfo::inited = false;
jclass UserInfo::clsZ = nullptr;

void UserInfo::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("com/fyber/fairbid/user/UserInfo");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);

            inited = true;
        }
    }
}

void UserInfo::setGdprConsent(jboolean haveConsent, jobject const &context) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && context!= nullptr){
        jmethodID method = jenv ->GetStaticMethodID(clsZ, "setGdprConsent", "(ZLandroid/content/Context;)V");
        jenv ->CallStaticVoidMethod(clsZ, method, haveConsent, context) ;
    }
}

void UserInfo::setLgpdConsent(jboolean haveConsent, jobject const &context) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && context!= nullptr){
        jmethodID method = jenv ->GetStaticMethodID(clsZ, "setLgpdConsent", "(ZLandroid/content/Context;)V");
        jenv ->CallStaticVoidMethod(clsZ, method, haveConsent, context) ;
    }
}
