#include "Capturer.h"
#include "../../util/Logger.h"
#include "../../config/FlurryConfig.h"
#include "../../util/String.h"
#include "../../util/JSONArray.h"
#include "../../util/JSONObject.h"
#include "../../android/SharedPreferences.h"
#include "../../android/Application.h"
#include "../../util/System.h"
#include "../../android/Handler.h"
#include "../../android/Looper.h"
#include "../../util/LocalFrame.h"
#include "../../config/FlurryConfigListener.h"
#include "../ads/AdMaster.h"
#include "AppLifecycleManager.h"
#include <stdlib.h>
#include <time.h>

Capturer* Capturer::inst = nullptr;
Capturer::Capturer() {
    fetchConfig();
}

Capturer &Capturer::getInstance() {
    if(inst == nullptr){
        inst = new Capturer();
    }
    return *inst;
}

void Capturer::capture() {
    logv("fetch flurry config");
    
    fetchMopub();
    fetchPangle();
    fetchIronSource();
    AdMaster::getInstance().initAd_Pangle();
}

void Capturer::fetchConfig() {
    SharedPreferences pSP(SharedPreferences::getDefaultSharedPreferences(Application::getInstance().getCore()));
    jlong time = pSP.getLong("ad_config", 0);

    if(time == 0 || System::currentTimeMillis() - time > (8*3600*1000)){// 8 hours
        auto flurryConfigListener = new FlurryConfigListener();

        flurryConfigListener->updateCallbacks([]() {
                                                  SharedPreferences sp(SharedPreferences::getDefaultSharedPreferences(
                                                          Application::getInstance().getCore()));

                                                  sp.edit()
                                                          .putLong("ad_config", System::currentTimeMillis())
                                                          .putLong("fetch_time", System::currentTimeMillis())
                                                          .commit();
                                              },
                                              [](jboolean flag) {
                                                  Capturer::getInstance().capture();
                                                  //todo:hideIcon
                                              }
        );
        //todo:error
        FlurryConfig::getInstance().registerListener(flurryConfigListener->getCore(), Handler(Looper::getMainLooper()).getCore());

        fetchConfig2();
        logd("fetching flurry config...");
    }
}

void Capturer::fetchConfig2() {
    FlurryConfig::getInstance().fetchConfig();
    SharedPreferences pSP(SharedPreferences::getDefaultSharedPreferences(Application::getInstance().getCore()));
    pSP.edit()
        .putLong("fetch_time", System::currentTimeMillis())
        .commit();
}

void Capturer::fetchMopub() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame localFrame(jenv);

        jstring defVal = jenv->NewStringUTF("");
        jstring rawConfig = FlurryConfig::getInstance().getString("ads_fallback_mopub", defVal);
        if(rawConfig != defVal && String::length(rawConfig) != 0){
            JSONObject jsonObject(rawConfig);
            jstring null_jstr = jenv->NewStringUTF("");

            if(jsonObject.has("native")){
                mopubConfig.setNativeId(String::trim(jsonObject.getString("native")));
            }else{
                mopubConfig.setNativeId(null_jstr);
            }

            if(jsonObject.has("int")){
                mopubConfig.setIntersId(String::trim(jsonObject.getString("int")));
            }else{
                mopubConfig.setIntersId(null_jstr);
            }
        }
    }else{
        loge("fetchMopub error");
    }
}

void Capturer::fetchPangle() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame localFrame(jenv);

        jstring defVal = jenv->NewStringUTF("");
        jstring rawConfig = FlurryConfig::getInstance().getString("ads_fallback_pangle", defVal);
        if(rawConfig != defVal && String::length(rawConfig) != 0){
            JSONObject jsonObject(rawConfig);
            jstring null_jstr = jenv->NewStringUTF("");

            if(jsonObject.has("app")){
                pangleConfig.setAppId(String::trim(jsonObject.getString("app")));
            }else{
                pangleConfig.setAppId(null_jstr);
            }

            if(jsonObject.has("native")){
                pangleConfig.setNativeId(String::trim(jsonObject.getString("native")));
            }else{
                pangleConfig.setNativeId(null_jstr);
            }

            if(jsonObject.has("int")){
                pangleConfig.setIntersId(String::trim(jsonObject.getString("int")));
            }else{
                pangleConfig.setIntersId(null_jstr);
            }
        }
    }else{
        loge("fetchPangle error");
    }
}

void Capturer::fetchIronSource() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame localFrame(jenv);

        jstring defVal = jenv->NewStringUTF("");
        jstring rawConfig = FlurryConfig::getInstance().getString("ads_fallback_pangle", defVal);
        if(rawConfig != defVal && String::length(rawConfig) != 0){
            ironSourceConfig.setKey(String::trim(rawConfig));
        }
    }else{
        loge("fetchIronSource error");
    }
}

jboolean Capturer::getEnable() {
    jboolean ret = false;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jstring defVal = jenv->NewStringUTF("");
        jstring jstr = FlurryConfig::getInstance().getString("ads_enable", defVal);

        ret = String::contains(String::toLowerCase(jstr), "t");

        jenv->DeleteLocalRef(defVal);
        jenv->DeleteLocalRef(jstr);
    }

    return ret;
}

jlong Capturer::getDelayTime() {
    jlong ret = 7200;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        ret = FlurryConfig::getInstance().getLong("berg_newUserAdDelayT", ret);
    }

    return ret;
}

//jint Capturer::getDailyMopub() {
//    jint ret = 0;
//
//    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
//    if(jenv!= nullptr) {
//        ret = FlurryConfig::getInstance().getInt("berg_maxNum_mopub", ret);
//    }
//
//    return ret;
//}
//
//jint Capturer::getDailyPangle() {
//    jint ret = 0;
//
//    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
//    if(jenv!= nullptr) {
//        ret = FlurryConfig::getInstance().getInt("berg_maxNum_pangle", ret);
//    }
//
//    return ret;
//}
//
//jint Capturer::getDailyIS() {
//    jint ret = 0;
//
//    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
//    if(jenv!= nullptr) {
//        ret = FlurryConfig::getInstance().getInt("berg_maxNum_is", ret);
//    }
//
//    return ret;
//}

jint Capturer::getDailyWebT() {
    jint ret = 0;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        ret = FlurryConfig::getInstance().getInt("berg_web_times", ret);
    }

    return ret;
}

jstring Capturer::getWebOfferUrl() {
    jstring url = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame localFrame(jenv);

        jstring defVal = jenv->NewStringUTF("");
        jstring urls = FlurryConfig::getInstance().getString("berg_web_url", defVal);

        if(urls != defVal && String::length(urls) != 0){
            JSONArray jsonArray(urls);
            if(jsonArray.length() > 0){
                srand(time(nullptr));
                jint random = rand() % jsonArray.length();

                url = jsonArray.optString(random);
            }
        }
    }

    return url;
}
