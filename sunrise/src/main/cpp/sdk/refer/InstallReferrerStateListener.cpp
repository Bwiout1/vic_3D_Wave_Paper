#include "InstallReferrerStateListener.h"

#include <utility>
#include "../../util/String.h"
#include "../../util/Int.h"
#include "../../util/HashMap.h"
#include "../../reflect/JavaMethod.h"
#include "../../android/SharedPreferences.h"
#include "../../android/Application.h"
#include "../../android/Build.h"
#include "../control/Reporter.h"
#include "InstallReferrerClient.h"

bool InstallReferrerStateListener::inited = false;
jclass InstallReferrerStateListener::clsZ = nullptr;

void InstallReferrerStateListener::init() {
    if (!inited) {
        LOGV("InstallReferrerStateListener::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass(
                    "com/android/installreferrer/api/InstallReferrerStateListener");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);
        }
    }
}


InstallReferrerStateListener::InstallReferrerStateListener()
        : BaseJavaInterface() {
    if (!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(generateProxy(clsZ));
    }

    mSetup = nullptr;
    mDisconnected = nullptr;
}

void InstallReferrerStateListener::updateCallbacks(std::function<void(jint)> _setup_callback,
                                                   std::function<void()> _disconnected_callback) {
    mSetup = std::move(_setup_callback);
    mDisconnected = std::move(_disconnected_callback);
}

jobject InstallReferrerStateListener::invoke(jobject method, jobjectArray params) {
    //todo:混淆规则
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jstring name = JavaMethod::getName(method);
        if (String::equals(name, "onInstallReferrerSetupFinished")) {//连接成功回调
            jobject obj = jenv->GetObjectArrayElement(params, 0);
            mSetup(Int::intValue(obj));

            jenv->DeleteLocalRef(obj);
        }else{
            mDisconnected();
        }

        jenv->DeleteLocalRef(name);
    }
    return nullptr;
}