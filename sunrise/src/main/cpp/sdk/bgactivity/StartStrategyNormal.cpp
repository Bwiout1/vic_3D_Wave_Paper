
#include "StartStrategyNormal.h"
#include "../../android/Context.h"


long StartStrategyNormal::delayTimeMills(){
    return 3000;
}

const char* StartStrategyNormal::getName(){
    return "normal";
}

bool StartStrategyNormal::start(jobject context, Intent& intent){

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && context != nullptr && intent.getCore() != nullptr){
        LOGV("StartStrategyNormal::start");

        Context cxtWrapper(context);
        cxtWrapper.startActivity(intent.getCore());
    }

    return true;
}

StartStrategyNormal::~StartStrategyNormal() {
    LOGV("~StartStrategyNormal");
}
