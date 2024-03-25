
#include "StartStrategySafeImpl.h"
#include "IntentUtils.h"

long StartStrategySafeImpl::delayTimeMills(){
    return 4000;
}

const char* StartStrategySafeImpl::getName(){
    return "safe";
}

bool StartStrategySafeImpl::start(jobject context, Intent& intent){
    bool flag = false;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && context != nullptr && intent.getCore() != nullptr){
        flag = IntentUtils::b(context, intent.getCore(), true);
        LOGV("StartStrategySafeImpl::start");
    }

    return flag;
}

StartStrategySafeImpl::~StartStrategySafeImpl() {
    LOGV("~StartStrategySafeImpl");
}
