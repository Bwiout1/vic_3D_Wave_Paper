#include "IronSit.h"
#include "../../util/PthreadJNIEnvHelper.h"
#include "../control/Record.h"
#include "../ads/mediationsdk/IronSource.h"
#include "../listener/InterstitialListener.h"
#include "../SunSdk.h"
#include <pthread.h>


IronSit* IronSit::inst = nullptr;

IronSit &IronSit::getInstance() {
    if(inst == nullptr){
        inst = new IronSit;
    }
    return *inst;
}

IronSit::IronSit() {
    std::atomic_init(&aInit, false);
    std::atomic_init(&aInters, false);
}

void IronSit::initNext() {
    aInit.store(true);
    loadInters();
}

jboolean IronSit::isInit() {
    return aInit.load();
}

jstring IronSit::name() {
    jstring ret = nullptr;

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        ret = jenv->NewStringUTF("ironSource");
    }
    return ret;
}

/*----------------------------------------native--------------------------------------------------*/

void IronSit::loadNative() {
/*nothing to do*/
}

void IronSit::fillNative(ViewGroup &container) {
/*nothing to do*/
}

jboolean IronSit::hasNative() {
    return false;
}

void IronSit::clearNative(ViewGroup &container) {
/*nothing to do*/
}

/*-------------------------------------interstitial-----------------------------------------------*/

void IronSit::onlyLoadInters(jobject const &activity) {

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        if(!jenv->IsInstanceOf(activity, SunSdk::getInstance().sunrise) || Record::getInstance().hasIronSource()){
            loadInters();
        }
    }
}

void IronSit::loadInters() {
    pthread_t tid;
    pthread_create(&tid, nullptr, p_loadInters, this);
}

void *IronSit::p_loadInters(void *args) {
    auto p = (IronSit*)args;
    if(!p->isInit()){
        return nullptr;
    }

    if(p->hasInters()){
        return nullptr;
    }

    if(!p->aInters.load()){
        p->aInters.store(true);

        IronSource::removeInterstitialListener();
        auto interstitialListener = new InterstitialListener("loadInters");
        IronSource::setInterstitialListener(interstitialListener->getCore());
        IronSource::loadInterstitial();

        logd("IronSource - inters load");
    }

    return nullptr;
}

void IronSit::showInters(jobject const &activity, IntersShowListener& callback) {
    if(hasInters()){
        *intersShowListener = callback;

        IronSource::removeInterstitialListener();
        auto interstitialListener = new InterstitialListener("showInters");
        IronSource::setInterstitialListener(interstitialListener->getCore());
        IronSource::showInterstitial();

        logd("IronSource - inters show");
    }else{
        loge("IronSource - inters show fail");

        callback.onShowFail(name());
        loadNextInters();
    }
}

void IronSit::loadNextInters() {

}

jboolean IronSit::hasInters() {
    return 0;
}
