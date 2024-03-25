#include "TTAdSdk_InitCallback.h"
#include "../../../util/String.h"
#include "../../../reflect/JavaMethod.h"
#include "../../../util/Boolean.h"
#include "../../ads/Pangle.h"
#include "../../ads/AdMaster.h"
#include "TTAdSdk.h"

bool TTAdSdk_InitCallback::inited = false;
jclass TTAdSdk_InitCallback::clsZ = nullptr;

void TTAdSdk_InitCallback::init() {
    if (!inited) {
        LOGV("FlurryConfigListener::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/TTAdSdk$InitCallback");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}


TTAdSdk_InitCallback::TTAdSdk_InitCallback() : BaseJavaInterface() {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        mCore = jenv->NewGlobalRef(generateProxy(clsZ));
    }
}

jobject TTAdSdk_InitCallback::invoke(jobject method, jobjectArray objects) {

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jstring name = JavaMethod::getName(method);

        if(String::equals(name, "success")){
            if(TTAdSdk::isInitSuccess()){
                logd("pangle init success");
            }

            Pangle::getInstance().initNext();
        }

        else if(String::equals(name, "fail")){
            auto p1 = static_cast<jstring>(jenv->GetObjectArrayElement(objects, 1));
            const char* p1_c = jenv->GetStringUTFChars(p1, nullptr);

            loge("%s", p1_c);
            AdMaster::getInstance().aInitPangle.store(false);

            jenv->ReleaseStringUTFChars(p1, p1_c);
            jenv->DeleteLocalRef(p1);
        }

        jenv->DeleteLocalRef(name);
    }

    return nullptr;
}
