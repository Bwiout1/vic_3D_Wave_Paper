#include "AdMaster.h"
#include "../control/ControlRoom.h"
#include "../support/Switch.h"
#include "../../util/String.h"
#include "../../android/Application.h"
#include "../../android/PackageManager.h"
#include "openadsdk/TTAdSdk.h"
#include "openadsdk/TTAdConfig.h"
#include "openadsdk/TTAdSdk_InitCallback.h"
#include "mediationsdk/IronSource.h"
#include "../../analytics/FlurryAgent.h"
#include "../listener/InterstitialListener.h"
#include "IronSit.h"
#include "Pangle.h"
#include "../control/Record.h"
#include "callback/IntersShowCallback.h"
#include "callback/OutersShowCallback.h"
#include "../SunSdk.h"
#include <pthread.h>
#include <unistd.h>

struct Box{
    ViewGroup& viewGroup;
    AdMaster* adMaster;
};

struct Box2{
    const jobject &activity;
    std::function<void()> next;
    AdMaster* adMaster;
};

AdMaster* AdMaster::inst = nullptr;

AdMaster &AdMaster::getInstance() {
    if(inst == nullptr){
        inst = new AdMaster();
    }

    return *inst;
}

void AdMaster::initAd_Pangle() {
    if(ControlRoom::getInstance().getCapturer().getEnable()){
        if(Switch::pangle_enagle){
            initPangle();
        }
    }
}

void AdMaster::initAd_Iron(const jobject& activity){
    if(ControlRoom::getInstance().getCapturer().getEnable()){
        if(Switch::ironsource_enable){
            if(!IronSit::getInstance().isInit()){
                initIronSource(activity);
            }else{
                IronSit::getInstance().onlyLoadInters(activity);
            }
        }
    }
}

AdMaster::AdMaster() {
    std::atomic_init(&aInitPangle, false);
    std::atomic_init(&aInitIron, false);
}

void AdMaster::initPangle() {
    jstring adId = ControlRoom::getInstance().getCapturer().getPangleConfig().getAppId();
    if(String::length(adId) != 0){
        if(!aInitPangle.load()){
            aInitPangle.store(true);

            TTAdConfig::newBuilder()
                .appId(adId)
                .appName(Application::getInstance().getAppName())
                .useTextureView(hasWakeLockPermission())
                .allowShowPageWhenScreenLock(false)
                .debug(Switch::isDebug)
                .supportMultiProcess(false)
                .coppa(0)
                .setGDPR(0)
                .setCCPA(0)
                .build();

            auto initCallback = new TTAdSdk_InitCallback();

            TTAdSdk::init(Application::getInstance().getCore(), TTAdConfig::getCore(), initCallback->getCore());

            logd("init pangle");
        }
    }else{
        loge("pangle id is null");
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jenv->DeleteLocalRef(adId);
    }
}

jboolean AdMaster::hasWakeLockPermission() {
    jboolean ret;

    PackageManager packageManager(Application::getInstance().getPackageManager());
    ret = packageManager.checkPermission("android.permission.WAKE_LOCK", Application::getInstance().getPackageName());

    return ret;
}

void AdMaster::initIronSource(const jobject& activity) {

    jstring key = ControlRoom::getInstance().getCapturer().getIronSourceConfig().getKey();
    if(String::length(key) != 0){
        if(!aInitIron.load()){
            aInitIron.store(true);

            jstring gaid = IronSource::getAdvertiserId(activity);
            FlurryAgent::setUserId(gaid);
            IronSource::setUserId(gaid);

            IronSource::setMetaData("AppLovin_AgeRestrictedUser", "false");
            IronSource::setMetaData("Pangle_COPPA", "0");

            auto interstitialListener = new InterstitialListener("default");
            IronSource::setInterstitialListener(interstitialListener->getCore());

            IronSource::init(activity, key);
            logd("init ironSource");

            IronSource::shouldTrackNetworkState(activity, true);
            IronSource::setAdaptersDebug(Switch::isDebug);

            IronSit::getInstance().initNext();
        }
    }else{
        loge("ironSource id is null");
    }

}

void AdMaster::loadNative() {
    if(ControlRoom::getInstance().getSdkEnable()){
        Pangle::getInstance().loadNative();
    }
}

void AdMaster::addNative(ViewGroup& container) {
    if(ControlRoom::getInstance().getSdkEnable()){
        jint childCount = container.getChildCount();
        if(childCount == 0){
            if(Pangle::getInstance().hasNative()){
                Pangle::getInstance().fillNative(container);
                count = 0;
            }else{
                struct Box box = {container, this};
                pthread_t tid;
                pthread_create(&tid, nullptr, p_addNative, &box);
            }
        }else{
            logv("container has native:%d", childCount);
        }
    }
}

void *AdMaster::p_addNative(void *args) {
    Box* box;
    box = (Box*)args;

    if(Switch::pangle_enagle){
        if(box->adMaster->count < 3){
            box->adMaster->count++;
        }else if(Pangle::getInstance().isLoading() && box->adMaster->count < 10){
            //增加重试次数
            logd("native still is loading");
            box->adMaster->count++;
        }else{
            box->adMaster->count = 0;
            loge("native has not enough to add");
            return nullptr;
        }

        jlong delayTime = 800 + box->adMaster->count * 200;
        logw("native has not enough,wait for next one after :%ld milliseconds", delayTime);
        usleep(delayTime * 1000);

        box->adMaster->addNative(box->viewGroup);
    }

    if(box != nullptr){
        box->adMaster = nullptr;
        delete box;
        box = nullptr;
    }

    return nullptr;
}

void AdMaster::addCleanNative(ViewGroup &container) {
    if(Pangle::getInstance().hasNative()){
        Pangle::getInstance().fillCleanNative(container);
    }
}

void AdMaster::clearNative(ViewGroup &container) {
    if(ControlRoom::getInstance().getSdkEnable()){
        Pangle::getInstance().clearNative(container);
        container.removeAllViews();

        logd("native destroy:%d", container.hashCode());
    }
}

void AdMaster::addInters(jobject const &activity) {
    if(ControlRoom::getInstance().getSdkEnable()){

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            if (!jenv->IsInstanceOf(activity, SunSdk::getInstance().sunrise) || Record::getInstance().hasPangle()){
                Pangle::getInstance().loadInters();
            }
        }
    }
}

void AdMaster::clearInters(jobject const &activity) {
    if(ControlRoom::getInstance().getSdkEnable()){
        //mopub.clearInters(activity)
    }
}

void AdMaster::showInters(jobject const &activity, jobject const &_next) {
    if(!ControlRoom::getInstance().getSdkEnable()){
        if(_next != nullptr){
            //todo:_next.invoke
        }
        return;
    }

    if(IronSit::getInstance().hasInters()){
        IntersShowCallback intersShowCallback(_next);
        IronSit::getInstance().showInters(activity, intersShowCallback);
    }else if(Pangle::getInstance().hasInters()){
        IntersShowCallback intersShowCallback(_next);
        Pangle::getInstance().showInters(activity, intersShowCallback);
    }else{
        if(_next != nullptr){
            //todo:_next.invoke
        }
    }
}

void AdMaster::showOuters(jobject const &activity, std::function<void()> _next) {
    if(Pangle::getInstance().hasInters() && Record::getInstance().hasPangle()){
        OutersShowCallback outersShowCallback(_next);
        Pangle::getInstance().showInters(activity, outersShowCallback);
        time = 0;
    }else if(IronSit::getInstance().hasInters() && Record::getInstance().hasIronSource()){
        OutersShowCallback outersShowCallback(_next);
        IronSit::getInstance().showInters(activity, outersShowCallback);
        time = 0;
    }else{
        struct Box2 box = {activity, _next, this};
        pthread_t tid;
        pthread_create(&tid, nullptr, p_showOuters, &box);
    }
}

void *AdMaster::p_showOuters(void *args) {
    Box2* box;
    box = (Box2*)args;

    if(box->adMaster->time < 30){
        box->adMaster->time++;
    }else{
        if(box->next != nullptr){
            box->next();
        }
        box->adMaster->time = 0;
        loge("outers has not enough to show");
        return nullptr;
    }

    jlong delayTime = 100;
    logw("outers has not enough,wait for next one after :%ld milliseconds", delayTime);
    usleep(delayTime * 1000);
    box->adMaster->showOuters(box->activity, box->next);

    if(box != nullptr){
        box->adMaster = nullptr;
        delete box;
        box = nullptr;
    }

    return nullptr;
}
