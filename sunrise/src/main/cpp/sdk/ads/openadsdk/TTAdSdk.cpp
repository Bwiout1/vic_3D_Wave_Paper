#include "TTAdSdk.h"
#include "../../../android/Application.h"
#include "TTAdNative.h"

bool TTAdSdk::inited = false;
jclass TTAdSdk::clsZ = nullptr;

void TTAdSdk::_init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/TTAdSdk");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

TTAdSdk::TTAdSdk() {
    _init();
}

TTAdSdk &TTAdSdk::getAdManager() {

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "getAdManager", "()Lcom/bytedance/sdk/openadsdk/TTAdManager;");
        jobject obj = jenv->CallStaticObjectMethod(clsZ, method);
        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }

    return *this;
}

TTAdNative *TTAdSdk::createAdNative() {
    TTAdNative* p = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jclass cls = jenv->FindClass("com/bytedance/sdk/openadsdk/TTAdManager");
        jmethodID method = jenv->GetMethodID(cls, "createAdNative", "(Landroid/content/Context;)Lcom/bytedance/sdk/openadsdk/TTAdNative;");
        jobject obj = jenv->CallObjectMethod(mCore, method, Application::getInstance().getCore());

        p = new TTAdNative(obj);

        jenv->DeleteLocalRef(cls);
        jenv->DeleteLocalRef(obj);
    }

    return p;
}

void TTAdSdk::init(jobject const &context, jobject const &ttAdConfig, jobject const &initCallBack) {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "init", "(Landroid/content/Context;Lcom/bytedance/sdk/openadsdk/TTAdConfig;Lcom/bytedance/sdk/openadsdk/TTAdSdk$InitCallback;)V");
        jenv->CallStaticVoidMethod(clsZ, method, context, ttAdConfig, initCallBack);
    }
}

jboolean TTAdSdk::isInitSuccess() {
    _init();

    jboolean ret = false;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "isInitSuccess", "()Z");
        ret = jenv->CallStaticBooleanMethod(clsZ, method);
    }

    return ret;
}
