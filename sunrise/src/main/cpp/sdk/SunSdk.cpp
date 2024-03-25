#include "SunSdk.h"
#include "../analytics/FacebookSdk.h"
#include "../analytics/YandexMetricaConfig.h"
#include "../analytics/YandexMetrica.h"
#include "support/Switch.h"
#include "../analytics/FlurryAgent.h"
#include "../util/LocalFrame.h"
#include "../util/Int.h"
#include "control/ControlRoom.h"

SunSdk *SunSdk::inst = nullptr;

SunSdk &SunSdk::getInstance() {
    if (inst == nullptr) {
        inst = new SunSdk();
    }

    return *inst;
}

SunSdk::SunSdk() {
    std::atomic_init(&lock, false);
}

void SunSdk::post(JNIEnv *env, jclass clazz, jobjectArray params) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        LocalFrame localFrame(jenv);

        jobject obj;

        obj = jenv->GetObjectArrayElement(params, 0);
        SunSdk::getInstance().worker = static_cast<jclass>(jenv->NewGlobalRef(obj));

        obj = jenv->GetObjectArrayElement(params, 1);
        SunSdk::getInstance().sunrise = static_cast<jclass>(jenv->NewGlobalRef(obj));

        obj = jenv->GetObjectArrayElement(params, 2);
        SunSdk::getInstance().layout_abc_tool_bar_layout = Int::intValue(obj);

        obj = jenv->GetObjectArrayElement(params, 3);
        SunSdk::getInstance().id_native_title = Int::intValue(obj);

        obj = jenv->GetObjectArrayElement(params, 4);
        SunSdk::getInstance().id_native_text = Int::intValue(obj);

        obj = jenv->GetObjectArrayElement(params, 5);
        SunSdk::getInstance().id_native_cta = Int::intValue(obj);

        obj = jenv->GetObjectArrayElement(params, 6);
        SunSdk::getInstance().id_native_icon_image = Int::intValue(obj);

        obj = jenv->GetObjectArrayElement(params, 7);
        SunSdk::getInstance().id_native_privacy_information_icon_image = Int::intValue(obj);

        obj = jenv->GetObjectArrayElement(params, 8);
        SunSdk::getInstance().id_native_main = Int::intValue(obj);

        obj = jenv->GetObjectArrayElement(params, 9);
        SunSdk::getInstance().id_native_main_media = Int::intValue(obj);

        obj = jenv->GetObjectArrayElement(params, 10);
        SunSdk::getInstance().id_blank = Int::intValue(obj);


        jenv->DeleteLocalRef(obj);

        SunSdk::getInstance().init();
    } else {
        LOGE("SunSdk::post error");
    }
}

void SunSdk::init() {
    if (!lock.load()) {
        lock.store(true);

        _init_analytics();

//        ControlRoom::getInstance().getBuilder()
//            .adLog()
//            .adConfig()
//            .cloak()
//            .run()
//            .build();
    }
}

void SunSdk::_init_analytics() {
    Application &app = Application::getInstance();

    if (!FacebookSdk::isInitialized()) {
        FacebookSdk::sdkInitialize(app);
    }

#if defined(Flurry_key)
    LOGV("init flurry, key is:%s", Flurry_key);

    FlurryAgent::Builder flurryAgentBuilder;
    flurryAgentBuilder
            .withLogEnabled(Switch::isDebug)
            .withLogLevel(Switch::isDebug ? 2 : 5)
            .withCaptureUncaughtExceptions(true)
            .build(app.getCore(), Flurry_key);
#else
#error "Flurry key is missed in build.gradle"
#endif

#if defined(Metrica_key)
    LOGV("init Yandex Appmetrica, key is:%s", Metrica_key);
    do {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv== nullptr)
            break;

        jobject configBuilderCore = YandexMetricaConfig::newConfigBuilder(Metrica_key);
        if (configBuilderCore == nullptr) {
            break;
        }

        jobject yandexConfigCore = YandexMetricaConfig::Builder(configBuilderCore)
                .withCrashReporting(true)
                .withNativeCrashReporting(true)
                .build();
        if (yandexConfigCore == nullptr) {
            break;
        }
        YandexMetrica::activate(app.getCore(), yandexConfigCore);

        YandexMetrica::enableActivityAutoTracking(app.getCore());
    } while (false);
#else
#error "AppMetrica key is missed in build.gradle"
#endif
}
