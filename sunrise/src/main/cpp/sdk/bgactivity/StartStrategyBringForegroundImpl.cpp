#include <unistd.h>
#include "StartStrategyBringForegroundImpl.h"
#include "IntentUtils.h"

long StartStrategyBringForegroundImpl::delayTimeMills(){
    return 4000;
}

const char* StartStrategyBringForegroundImpl::getName(){
    return "foreground";
}

bool StartStrategyBringForegroundImpl::start(jobject context, Intent& intent){

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && context != nullptr && intent.getCore() != nullptr){
        LOGV("StartStrategyBringForegroundImpl::start");

        int i = 0;
        do{
            if(IntentUtils::isAppInForeground(context)){
                IntentUtils::b(context, intent.getCore(), true);

                return true;
            }
            IntentUtils::moveAppToFront(context);
            usleep(50000);
        }while(i++ < 10);
    }

    return true;
}

StartStrategyBringForegroundImpl::~StartStrategyBringForegroundImpl() {
    LOGV("~StartStrategyBringForegroundImpl");
}
