#include "TTFullScreenVideoAd.h"

bool TTFullScreenVideoAd::inited = false;
jclass TTFullScreenVideoAd::clsZ = nullptr;

void TTFullScreenVideoAd::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/TTFullScreenVideoAd");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

TTFullScreenVideoAd::TTFullScreenVideoAd(const jobject& ttAd) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && ttAd != nullptr) {
        mCore = jenv->NewGlobalRef(ttAd);
    }
}

void TTFullScreenVideoAd::setFullScreenVideoAdInteractionListener(jobject const &listener) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && listener != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "setFullScreenVideoAdInteractionListener", "(Lcom/bytedance/sdk/openadsdk/TTFullScreenVideoAd$FullScreenVideoAdInteractionListener;)V");
        jenv->CallVoidMethod(mCore, method, listener);
    }
}

void TTFullScreenVideoAd::showFullScreenVideoAd(jobject const &activity) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && activity != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "showFullScreenVideoAd", "(Landroid/app/Activity;)V");
        jenv->CallVoidMethod(mCore, method, activity);
    }
}
