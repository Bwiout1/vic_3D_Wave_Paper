#include "IronSource.h"

bool IronSource::inited = false;
jclass IronSource::clsZ = nullptr;

void IronSource::_init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/ironsource/mediationsdk/IronSource");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

void IronSource::removeInterstitialListener() {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "removeInterstitialListener", "()V");
        jenv->CallStaticVoidMethod(clsZ, method);
    }
}

void IronSource::setInterstitialListener(jobject const &listener) {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && listener != nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "setInterstitialListener", "(Lcom/ironsource/mediationsdk/sdk/InterstitialListener;)V");
        jenv->CallStaticVoidMethod(clsZ, method, listener);
    }
}

void IronSource::loadInterstitial() {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "loadInterstitial", "()V");
        jenv->CallStaticVoidMethod(clsZ, method);
    }
}

void IronSource::showInterstitial() {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "showInterstitial", "()V");
        jenv->CallStaticVoidMethod(clsZ, method);
    }
}

jstring IronSource::getAdvertiserId(jobject const &activity) {
    _init();
    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "getAdvertiserId", "(Landroid/content/Context;)Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->CallStaticObjectMethod(clsZ, method, activity));
    }

    return ret;
}

void IronSource::setUserId(jstring jstr) {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "setUserId", "(Ljava/lang/String;)V");
        jenv->CallStaticVoidMethod(clsZ, method, jstr);
    }
}

void IronSource::setMetaData(const char *str1, const char *str2) {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jstring jstr1 = jenv->NewStringUTF(str1);
        jstring jstr2 = jenv->NewStringUTF(str2);
        jmethodID method = jenv->GetStaticMethodID(clsZ, "setMetaData", "(Ljava/lang/String;Ljava/lang/String;)V");
        jenv->CallStaticVoidMethod(clsZ, method, jstr1, jstr2);

        jenv->DeleteLocalRef(jstr1);
        jenv->DeleteLocalRef(jstr2);
    }
}

void IronSource::init(jobject const &activity, jstring jstr) {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "init", "(Landroid/app/Activity;Ljava/lang/String;)V");
        jenv->CallStaticVoidMethod(clsZ, method, activity, jstr);
    }
}

void IronSource::shouldTrackNetworkState(jobject const &activity, jboolean flag) {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "shouldTrackNetworkState",
                                                   "(Landroid/content/Context;Z)V");
        jenv->CallStaticVoidMethod(clsZ, method, activity, flag);
    }
}

void IronSource::setAdaptersDebug(jboolean flag) {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "setAdaptersDebug",
                                                   "(Z)V");
        jenv->CallStaticVoidMethod(clsZ, method, flag);
    }
}

void IronSource::onResume(jobject const &activity) {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "onResume",
                                                   "(Landroid/app/Activity;)V");
        jenv->CallStaticVoidMethod(clsZ, method, activity);
    }
}

void IronSource::onPause(jobject const &activity) {
    _init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "onPause",
                                                   "(Landroid/app/Activity;)V");
        jenv->CallStaticVoidMethod(clsZ, method, activity);
    }
}
