#include "TestSuit.h"
#include <jni.h>
#include <pthread.h>
#include <string>
#include "../sdk/SunSdk.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../config/FlurryConfigListener.h"
#include "../config/FlurryConfig.h"
#include "../reflect/JavaClass.h"
#include "../sdk/control/ControlRoom.h"
#include "../util/Logger.h"
#include "../sdk/refer/InstallReferrerClient.h"
#include "../sdk/refer/InstallReferrerStateListener.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_sunrise_sunrise_SunSpot_test(JNIEnv *env, jclass clazz, jstring id) {
//    SunSdk sunSdk = SunSdk::getInstance();
//    sunSdk.init();
//
//    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
//    if(jenv!= nullptr) {
//        auto* p = new FlurryConfigListener();
//        FlurryConfig::getInstance().registerListener(p->getCore());
//    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        const char *id_c = jenv->GetStringUTFChars(id, nullptr);
        if (std::string(id_c).empty()) {
            LOGE("is empty");
        } else {
            LOGE("not empty");
        }
    }


}

void *TestSuit::p_test(void *args) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jclass cls = jenv->FindClass("java/lang/String");
        if (cls != nullptr) {
            LOGE("cls is here");
        } else {
            LOGE("cls is null");
        }

    }

    return nullptr;
}

namespace TestSuit {
    void _test_gp_InstallReferrerClient() {
        Application &appInst = Application::getInstance();

        InstallReferrerClient::Builder builder(
                InstallReferrerClient::newBuilder(appInst.getCore())
        );
        auto *referrerClient = new InstallReferrerClient(builder.build());

        auto *referrerStatListener = new InstallReferrerStateListener();
        referrerStatListener->updateCallbacks([referrerClient](jint code) {

                                                  if(InstallReferrerClient::InstallReferrerResponse::OK == code){
                                                      jstring referrer = referrerClient->installReferrer();
                                                  }
                                                  referrerClient->endConnection();
                                              },
                                              [referrerClient, referrerStatListener]() {
                                                  DeferredGC::recycle(referrerClient);
                                                  DeferredGC::recycle(referrerStatListener);
                                              });
        referrerClient->startConnection(referrerStatListener->getCore());
    }

    void run() {
        _test_gp_InstallReferrerClient();
    }
}
