#include "FlurryAgent.h"

bool FlurryAgent::inited = false;
jclass FlurryAgent::clsZ = nullptr;

void FlurryAgent::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("com/flurry/android/FlurryAgent");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv->DeleteLocalRef(tmp);
            inited = true;
        }
    }
}

void FlurryAgent::logEvent(jstring jstr) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "logEvent", "(Ljava/lang/String;)Lcom/flurry/android/FlurryEventRecordStatus;");
        jenv->CallStaticObjectMethod(clsZ, method, jstr);
    }
}

void FlurryAgent::logEvent(jstring jstr, HashMap &map) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "logEvent", "(Ljava/lang/String;Ljava/util/Map;)Lcom/flurry/android/FlurryEventRecordStatus;");
        jenv->CallStaticObjectMethod(clsZ, method, jstr, map.getCore());
    }
}

void FlurryAgent::setUserId(jstring jstr) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "setUserId", "(Ljava/lang/String;)V");
        jenv->CallStaticVoidMethod(clsZ, method, jstr);
    }
}


////////////////////////////FlurryAgent::Builder////////////////////////////////////////////////////
bool FlurryAgent::Builder::inited = false;
jclass FlurryAgent::Builder::clsZ = nullptr;


void FlurryAgent::Builder::init() {
    if(!inited){
        LOGV("FlurryAgent::Builder::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("com/flurry/android/FlurryAgent$Builder");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);

            inited = true;
        }
    }
}

FlurryAgent::Builder::Builder() {
    if(!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "()V");

        mCore = jenv->NewGlobalRef(
                jenv->NewObject(clsZ, method)
        );
    }
}

FlurryAgent::Builder &FlurryAgent::Builder::withLogEnabled(jboolean flag) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "withLogEnabled",
                                             "(Z)Lcom/flurry/android/FlurryAgent$Builder;");

        jenv->CallObjectMethod(mCore, method, flag);
    }

    return *this;
}

FlurryAgent::Builder &FlurryAgent::Builder::withLogLevel(jint i) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "withLogLevel", "(I)Lcom/flurry/android/FlurryAgent$Builder;");

        jenv->CallObjectMethod(mCore, method, i);
    }

    return *this;
}

FlurryAgent::Builder &FlurryAgent::Builder::withCaptureUncaughtExceptions(jboolean flag) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "withCaptureUncaughtExceptions",
                                             "(Z)Lcom/flurry/android/FlurryAgent$Builder;");

        jenv->CallObjectMethod(mCore, method, flag);
    }

    return *this;
}

void FlurryAgent::Builder::build(const jobject & context, const char* flurrykey) {

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "build", "(Landroid/content/Context;Ljava/lang/String;)V");

        jstring jstr = jenv->NewStringUTF(flurrykey);
        jenv->CallVoidMethod(mCore, method, context, jstr);

        jenv->DeleteLocalRef(jstr);
    }
}