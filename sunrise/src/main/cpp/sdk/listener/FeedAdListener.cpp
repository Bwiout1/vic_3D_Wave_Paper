#include "FeedAdListener.h"
#include "../../util/String.h"
#include "../../util/Int.h"
#include "../../reflect/JavaMethod.h"
#include "../ads/Pangle.h"

bool FeedAdListener::inited = false;
jclass FeedAdListener::clsZ = nullptr;

void FeedAdListener::init() {
    if (!inited) {
        LOGV("FeedAdListener::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/TTAdNative$FeedAdListener");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}


FeedAdListener::FeedAdListener() : BaseJavaInterface() {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        mCore = jenv->NewGlobalRef(generateProxy(clsZ));
    }
}

jobject FeedAdListener::invoke(jobject method, jobjectArray objects) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jstring name = JavaMethod::getName(method);

        if(String::equals(name, "onError")){
            auto p1 = static_cast<jstring>(jenv->GetObjectArrayElement(objects, 1));
            const char* p1_c = jenv->GetStringUTFChars(p1, nullptr);
            loge("native load fail:%s", p1_c);

            Pangle::getInstance().aNative.store(false);

            jenv->ReleaseStringUTFChars(p1, p1_c);
            jenv->DeleteLocalRef(p1);
        }

        else if(String::equals(name, "onFeedAdLoad")){
            Pangle::getInstance().mCacheInters->addAll(jenv->GetObjectArrayElement(objects, 0));
            Pangle::getInstance().aNative.store(false);
        }

        jenv->DeleteLocalRef(name);
    }
    return nullptr;
}
