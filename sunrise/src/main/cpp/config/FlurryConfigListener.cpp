#include "FlurryConfigListener.h"
#include "../util/String.h"
#include "../reflect/JavaMethod.h"
#include "../android/SharedPreferences.h"
#include "../android/Application.h"
#include "../util/System.h"
#include "../util/Boolean.h"
#include "../sdk/control/Capturer.h"
#include "FlurryConfig.h"

bool FlurryConfigListener::inited = false;
jclass FlurryConfigListener::clsZ = nullptr;

void FlurryConfigListener::init() {
    if (!inited) {
        LOGV("FlurryConfigListener::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/flurry/android/FlurryConfigListener");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}


FlurryConfigListener::FlurryConfigListener() : BaseJavaInterface() {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        mCore = jenv->NewGlobalRef(generateProxy(clsZ));
    }

    fetchSuccess = nullptr;
    activateComplete = nullptr;
}

void FlurryConfigListener::updateCallbacks(std::function<void()> _fetchSuccess,
                                           std::function<void(jboolean)> _activateComplete) {
    fetchSuccess = std::move(_fetchSuccess);
    activateComplete = std::move(_activateComplete);
}

jobject FlurryConfigListener::invoke(jobject method, jobjectArray params) {
    //防止混淆方法名
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {

        jstring name = JavaMethod::getName(method);
        if(String::equals(name, "onFetchSuccess")){
            onFetchSuccess();

            fetchSuccess();
        }

        else if (String::equals(name, "onFetchError")) {
            jobject obj = jenv->GetObjectArrayElement(params, 0);
            onFetchError(Boolean::booleanValue(obj)) ;

            jenv->DeleteLocalRef(obj);
        }

        else if (String::equals(name, "onActivateComplete")) {
            jobject obj = jenv->GetObjectArrayElement(params, 0);
            onActivateComplete(Boolean::booleanValue(obj)) ;

            activateComplete(Boolean::booleanValue(obj));
        }

        jenv->DeleteLocalRef(name);
    }
    return nullptr;
}

void FlurryConfigListener::onFetchSuccess() {
    FlurryConfig::getInstance().activateConfig();
}

void FlurryConfigListener::onFetchNoChange() {
}

void FlurryConfigListener::onFetchError(jboolean isRetrying) {
    loge("fetch flurry error:%s", isRetrying ? "true" : "false");
}

void FlurryConfigListener::onActivateComplete(jboolean isCache) {

}
