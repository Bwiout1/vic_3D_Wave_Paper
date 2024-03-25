#include "MediationStartedListener.h"

bool MediationStartedListener::inited = false;
jclass MediationStartedListener::clsZ = nullptr;

void MediationStartedListener::init() {
    if (!inited) {
        LOGV("MediationStartedListener::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass(
                    "com/fyber/fairbid/ads/mediation/MediationStartedListener");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);
        }
    }
}


MediationStartedListener::MediationStartedListener(
        MediationStartedListener::StartCallback start_callback,
        MediationStartedListener::FailedCallback failed_callback) : BaseJavaInterface() {
    if (!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(generateProxy(clsZ));

        mStartCallback = start_callback;
        mFailedCallback = failed_callback;
    }
}


jobject MediationStartedListener::invoke(jobject method, jobjectArray objects) {
    return nullptr;
}
