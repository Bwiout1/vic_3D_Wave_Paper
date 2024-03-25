#include "InterstitialListener.h"
#include "../../util/String.h"
#include "../../util/HashMap.h"
#include "../../util/LocalFrame.h"
#include "../../android/Process.h"
#include "../../reflect/JavaMethod.h"
#include "../ads/mediationsdk/IronSourceError.h"
#include "../ads/IronSit.h"
#include <string>

bool InterstitialListener::inited = false;
jclass InterstitialListener::clsZ = nullptr;

void InterstitialListener::init() {
    if (!inited) {
        LOGV("AdInteractionListener::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/ironsource/mediationsdk/sdk/InterstitialListener");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}


InterstitialListener::InterstitialListener(const char* _kind) : BaseJavaInterface() {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        kind = _kind;
        mCore = jenv->NewGlobalRef(generateProxy(clsZ));
    }
}

jobject InterstitialListener::invoke(jobject method, jobjectArray objects) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame localFrame(jenv);

        jstring name = JavaMethod::getName(method);

        if(String::equals(name, "onInterstitialAdReady")){
            logd("IronSource - inters loaded");

            if(std::string(kind) == "loadInters"){
                IronSit::getInstance().aInters.store(false);
            }
        }

        if(String::equals(name, "onInterstitialAdLoadFailed")){
            jobject error = jenv->GetObjectArrayElement(objects, 0);

            if(error != nullptr){
                IronSourceError ironSourceError(error);

                jstring jstr = ironSourceError.toString();
                const char* jstr_c = jenv->GetStringUTFChars(jstr, nullptr);

                loge("IronSource - inters failed:%s", jstr_c);

                jenv->ReleaseStringUTFChars(jstr, jstr_c);
                jenv->DeleteLocalRef(jstr);


                if(ironSourceError.getErrorCode() == IronSourceError::ERROR_IS_LOAD_DURING_SHOW){
                    jenv->DeleteLocalRef(error);

                    Process::killProcess(Process::myPid());
                }

                if(std::string(kind) == "loadInters"){
                    IronSit::getInstance().aInters.store(false);
                }
            }

        }

        else if(String::equals(name, "onInterstitialAdOpened")){
            logd("IronSource - inters open");
        }

        else if(String::equals(name, "onInterstitialAdClosed")){
            logd("IronSource - inters close");

            if(std::string(kind) == "showInters"){
                IronSit::getInstance().intersShowListener->onDismiss(IronSit::getInstance().name());
                IronSit::getInstance().loadNextInters();
            }
        }

        else if(String::equals(name, "onInterstitialAdShowSucceeded")){
            logd("IronSource - inters shown");

            if(std::string(kind) == "showInters"){
                IronSit::getInstance().intersShowListener->onShow(IronSit::getInstance().name());
            }
        }

        else if(String::equals(name, "onInterstitialAdShowFailed")){
            jobject error = jenv->GetObjectArrayElement(objects, 0);

            if(error != nullptr) {
                IronSourceError ironSourceError(error);

                jstring jstr = ironSourceError.toString();
                const char* jstr_c = jenv->GetStringUTFChars(jstr, nullptr);

                loge("IronSource - inters show failed:%s", jstr_c);

                jenv->ReleaseStringUTFChars(jstr, jstr_c);
                jenv->DeleteLocalRef(jstr);
                jenv->DeleteLocalRef(error);
            }

            if(std::string(kind) == "showInters"){
                IronSit::getInstance().intersShowListener->onShowFail(IronSit::getInstance().name());
                IronSit::getInstance().loadNextInters();
            }
        }

        else if(String::equals(name, "onInterstitialAdClicked")){
            logd("IronSource - inters click");
        }

        jenv->DeleteLocalRef(name);
    }

    return nullptr;
}
