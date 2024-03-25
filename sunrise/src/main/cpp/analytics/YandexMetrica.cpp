#include "YandexMetrica.h"

bool YandexMetrica::inited = false;
jclass YandexMetrica::clsZ = nullptr;

void YandexMetrica::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("com/yandex/metrica/YandexMetrica");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);

            inited = true;
        }
    }
}

void YandexMetrica::activate(jobject const &app, jobject const &config) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "activate", "(Landroid/content/Context;Lcom/yandex/metrica/YandexMetricaConfig;)V");
        jenv->CallStaticVoidMethod(clsZ, method, app, config);
    }
}

void YandexMetrica::enableActivityAutoTracking(jobject const &app) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "enableActivityAutoTracking", "(Landroid/app/Application;)V");
        jenv->CallStaticVoidMethod(clsZ, method, app);
    }
}

void YandexMetrica::reportEvent(jstring jstr) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "reportEvent", "(Ljava/lang/String;)V");
        jenv->CallStaticVoidMethod(clsZ, method, jstr);
    }
}

void YandexMetrica::reportEvent(jstring jstr, HashMap &map) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "reportEvent", "(Ljava/lang/String;Ljava/util/Map;)V");
        jenv->CallStaticVoidMethod(clsZ, method, jstr, map.getCore());
    }
}
