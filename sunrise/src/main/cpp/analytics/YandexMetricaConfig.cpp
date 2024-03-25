#include "YandexMetricaConfig.h"

bool YandexMetricaConfig::inited = false;
jclass YandexMetricaConfig::clsZ = nullptr;

void YandexMetricaConfig::init() {
    if (!inited) {
        LOGV("YandexMetricaConfig::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/yandex/metrica/YandexMetricaConfig");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jobject YandexMetricaConfig::newConfigBuilder(const char *_key) {
    if (!inited) {
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && _key!= nullptr) {
        jmethodID method = jenv ->GetStaticMethodID(clsZ,
                                                    "newConfigBuilder",
                                                    "(Ljava/lang/String;)Lcom/yandex/metrica/YandexMetricaConfig$Builder;");
        ret = jenv ->CallStaticObjectMethod(clsZ, method, jenv ->NewStringUTF(_key));
    }

    return ret;
}

/*-----------------------------------------Builder------------------------------------------------*/
bool YandexMetricaConfig::Builder::inited = false;
jclass YandexMetricaConfig::Builder::clsZ = nullptr;

void YandexMetricaConfig::Builder::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("com/yandex/metrica/YandexMetricaConfig$Builder");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);

            inited = true;
        }
    }
}

YandexMetricaConfig::Builder::Builder(const jobject core) {
    if(!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core!= nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

jobject YandexMetricaConfig::Builder::build() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "build", "()Lcom/yandex/metrica/YandexMetricaConfig;");
        ret = jenv->CallObjectMethod(mCore, method);
    }

    return ret;
}

YandexMetricaConfig::Builder &YandexMetricaConfig::Builder::withCrashReporting(jboolean flag) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "withCrashReporting",
                                             "(Z)Lcom/yandex/metrica/YandexMetricaConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, flag);
    }

    return *this;
}

YandexMetricaConfig::Builder &
YandexMetricaConfig::Builder::withNativeCrashReporting(jboolean flag) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "withNativeCrashReporting",
                                             "(Z)Lcom/yandex/metrica/YandexMetricaConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, flag);
    }

    return *this;
}
