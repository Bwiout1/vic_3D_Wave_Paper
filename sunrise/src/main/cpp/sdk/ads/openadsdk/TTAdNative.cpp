#include "TTAdNative.h"

bool TTAdNative::inited = false;
jclass TTAdNative::clsZ = nullptr;

void TTAdNative::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/TTAdNative");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

TTAdNative::TTAdNative(jobject const &core) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

void TTAdNative::loadFeedAd(jobject const &adSlot, jobject listener) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "loadFeedAd", "(Lcom/bytedance/sdk/openadsdk/AdSlot;Lcom/bytedance/sdk/openadsdk/TTAdNative$FeedAdListener;)V");
        jenv->CallVoidMethod(mCore, method, adSlot, listener);
    }
}

void TTAdNative::loadFullScreenVideoAd(jobject const &adSlot, jobject listener) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "loadFullScreenVideoAd", "(Lcom/bytedance/sdk/openadsdk/AdSlot;Lcom/bytedance/sdk/openadsdk/TTAdNative$FullScreenVideoAdListener;)V");
        jenv->CallVoidMethod(mCore, method, adSlot, listener);
    }
}
