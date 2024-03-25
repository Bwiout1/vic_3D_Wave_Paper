#include "FairBid.h"


bool FairBid::inited = false;
jclass FairBid::clsZ = nullptr;

void FairBid::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("com/fyber/FairBid");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);

            inited = true;
        }
    }
}

jobject FairBid::configureForAppId(const char *app_id) {
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && app_id!= nullptr){
        jmethodID method = jenv ->GetStaticMethodID(clsZ, "configureForAppId", "(Ljava/lang/String;)Lcom/fyber/FairBid;") ;

        jstring appId = jenv ->NewStringUTF(app_id) ;
        ret = jenv ->CallStaticObjectMethod(clsZ, method, appId) ;
        jenv ->ReleaseStringUTFChars(appId, app_id) ;
    }

    return ret ;
}

jboolean FairBid::hasStarted() {
    if(!inited){
        init();
    }

    jboolean ret = false;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv ->GetStaticMethodID(clsZ, "hasStarted", "()Z");
        ret = jenv ->CallStaticBooleanMethod(clsZ, method) ;
    }

    return ret;
}

FairBid::FairBid(const jobject core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        mCore = jenv ->NewGlobalRef(core) ;
    }
}

FairBid &FairBid::enableLogs() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "enableLogs", "()Lcom/fyber/FairBid;");
        jenv ->CallObjectMethod(mCore, method);
    }

    return *this;
}

FairBid &FairBid::setUserAChild(jboolean child) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setUserAChild", "(Z)Lcom/fyber/FairBid;");
        jenv ->CallObjectMethod(mCore, method, child);
    }

    return *this;
}

FairBid &FairBid::withMediationStartedListener(jobject listener) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ,
                                              "withMediationStartedListener",
                                              "(Lcom/fyber/fairbid/ads/mediation/MediationStartedListener;)Lcom/fyber/FairBid;");
        jenv ->CallObjectMethod(mCore, method, listener);
    }

    return *this;
}

void FairBid::start(jobject activity) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr && activity!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "start", "(Landroid/app/Activity;)V");
        jenv ->CallVoidMethod(mCore, method, activity);
    }
}





