#include "BrainWave.h"
#include "../../android/SharedPreferences.h"
#include "../../android/Application.h"
#include "../../android/AndroidJni.h"
#include "../../util/PthreadJNIEnvHelper.h"
#include "../../util/LocalFrame.h"
#include "../../util/String.h"
#include "../../util/System.h"
#include "../../util/Runnable.h"
#include "../../util/HashMap.h"
#include "../refer/InstallReferrerClient.h"
#include "../refer/InstallReferrerStateListener.h"
#include "../support/Switch.h"
#include "../control/ControlRoom.h"
#include "Reporter.h"
#include "../../android/Build.h"
#include "../../util/Int.h"
#include <pthread.h>
#include <string>

BrainWave* BrainWave::inst = nullptr;

BrainWave &BrainWave::getInstance() {
    if(inst == nullptr){
        inst = new BrainWave();
    }

    return *inst;
}

BrainWave::BrainWave() {

}

jboolean BrainWave::isCloak() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame localFrame(jenv);

        SharedPreferences sp(SharedPreferences::getDefaultSharedPreferences(Application::getInstance().getCore()));
        jstring ref = sp.getString("cloak", jenv->NewStringUTF(""));

        if(String::length(ref) == 0){
            jlong nowTime = System::currentTimeMillis();
            if(nowTime - logTime0 > 2000){
                logd("ref is null");
            }
            logTime0 = nowTime;

            return true;
        }

        const char* ref_c = jenv->GetStringUTFChars(ref, nullptr);
        bool organic = std::string(ref_c).find("organic") != -1;

        jenv->ReleaseStringUTFChars(ref, ref_c);

        if(Switch::isDebug){
            jlong nowTime = System::currentTimeMillis();
            if(nowTime - logTime1 > 5000){
                logd("for debug, change organic from %s to false", organic? "true":"false");
            }
            logTime1 = nowTime;

            return false;
        }

        return organic;
    }

    return true;
}

void BrainWave::checkCloak() {
    pthread_t tid;
    pthread_create(&tid, nullptr, p_checkCloak, nullptr);
}

void* BrainWave::p_checkCloak(void* args) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame localFrame(jenv);

        SharedPreferences sp(SharedPreferences::getDefaultSharedPreferences(Application::getInstance().getCore()));
        jstring ref = sp.getString("cloak", jenv->NewStringUTF(""));

        if(String::length(ref) == 0){
            Application &appInst = Application::getInstance();

            InstallReferrerClient::Builder builder(
                    InstallReferrerClient::newBuilder(appInst.getCore())
            );
            auto *referrerClient = new InstallReferrerClient(builder.build());

            auto *referrerStatListener = new InstallReferrerStateListener();
            referrerStatListener->updateCallbacks([referrerClient](jint code) {

                                                      if(InstallReferrerClient::InstallReferrerResponse::OK == code){
                                                          JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
                                                          if (jenv != nullptr) {
                                                              LocalFrame localFrame(jenv);

                                                              jstring referrer = referrerClient->installReferrer();
                                                              const char *_referrer = jenv->GetStringUTFChars(referrer, nullptr);
                                                              logv("referrer: %s", _referrer);

                                                              SharedPreferences sp(SharedPreferences::getDefaultSharedPreferences(
                                                                      Application::getInstance().getCore()));
                                                              sp.edit()
                                                                      .putString("cloak", referrer)
                                                                      .commit();

                                                              HashMap hashMap = HashMap();
                                                              jstring jstr_ref = jenv->NewStringUTF("ref");
                                                              jstring jstr_dev = jenv->NewStringUTF("dev");

                                                              jstring dev_value = getDevPrint();
                                                              if (dev_value == nullptr) {
                                                                  loge("dev_value is null");
                                                              }
                                                              hashMap.put(jstr_ref, referrer)
                                                                      .put(jstr_dev, dev_value);

                                                              Reporter::report("binstall", hashMap);
                                                              //todo:isCloak中重新从sp获取refer, hideIcon

                                                              jenv->ReleaseStringUTFChars(referrer, _referrer);
                                                          }
                                                      }
                                                      referrerClient->endConnection();
                                                  },
                                                  [referrerClient, referrerStatListener]() {
                                                      DeferredGC::recycle(referrerClient);
                                                      DeferredGC::recycle(referrerStatListener);
                                                  });
            referrerClient->startConnection(referrerStatListener->getCore());
        }

        jenv->DeleteLocalRef(ref);
    }
    return nullptr;
}

void BrainWave::hide() {
    auto run = new Runnable((RunnableCallback)BrainWave::_hide, this);
    AndroidJni::getUIHandler().postDelayed(*run, 5000);
}

void BrainWave::_hide(BrainWave* ptr) {
    if(!ControlRoom::getInstance().getSdkEnable()){
        return;
    }
    //todo:lAddShortcut
}

jstring BrainWave::getDevPrint() {
    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jclass cls = jenv->FindClass("java/lang/Object");
        jobjectArray arr = jenv->NewObjectArray(3, cls, nullptr);

        jenv->SetObjectArrayElement(arr, 0, Build::MANUFACTURER());
        jenv->SetObjectArrayElement(arr, 1, Build::MODEL());
        Int _int(Build::SDK_INT());
        jenv->SetObjectArrayElement(arr, 2, _int.getCore());

        ret = String::format("%s|%s|%s", arr);

        jenv->DeleteLocalRef(cls);
        jenv->DeleteLocalRef(arr);
    }

    return ret;
}
