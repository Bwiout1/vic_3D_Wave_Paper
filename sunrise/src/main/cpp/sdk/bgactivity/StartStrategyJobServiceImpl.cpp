#include "StartStrategyJobServiceImpl.h"
#include "../../util/RomUtils.h"
#include "../../android/AndroidJni.h"
#include "IntentUtils.h"


long StartStrategyJobServiceImpl::delayTimeMills(){
    return 2000;
}

const char* StartStrategyJobServiceImpl::getName(){
    return "jobService";
}

bool StartStrategyJobServiceImpl::start(jobject context, Intent& intent){

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && intent.getCore() != nullptr){
        if(RomUtils::isVivo() && AndroidJni::get_dev_api_level() >= 26){
            IntentUtils::moveAppToFront(context);
        }

        jclass cls = jenv->FindClass("java/lang/MyJobService");
        jmethodID mid = jenv->GetStaticMethodID(cls, "startJob", "(Landroid/content/Context;Landroid/content/Intent;)V");
        jenv->CallStaticVoidMethod(cls, mid, context, intent.getCore());

        LOGV("StartStrategyJobServiceImpl::start");
        jenv->DeleteLocalRef(cls);
    }

    return true;
}

StartStrategyJobServiceImpl::~StartStrategyJobServiceImpl() {
    LOGV("~StartStrategyJobServiceImpl");
}

