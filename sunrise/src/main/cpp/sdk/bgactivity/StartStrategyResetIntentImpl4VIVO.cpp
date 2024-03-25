#include "StartStrategyResetIntentImpl4VIVO.h"
#include "../../util/JniException.h"


long StartStrategyResetIntentImpl4VIVO::delayTimeMills(){
    return 0;
}

const char* StartStrategyResetIntentImpl4VIVO::getName(){
    return "ResetIntentV";
}

bool StartStrategyResetIntentImpl4VIVO::start(jobject context, Intent& intent){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && intent.getCore() != nullptr){
        jclass cls = jenv->GetObjectClass(intent.getCore());
        jfieldID fid = jenv->GetFieldID(cls, "mIsVivoWidget", "Z");
        if(!check_and_clear_exception(jenv)){
            jenv->SetBooleanField(intent.getCore(), fid, true);
            LOGV("StartStrategyResetIntentImpl4VIVO::start, intent.mIsVivoWidget = true");
        } else {
            LOGV("StartStrategyResetIntentImpl4VIVO::start, it isn't vivo rom") ;
        }

        jenv->DeleteLocalRef(cls);
    }

    return true;
}

StartStrategyResetIntentImpl4VIVO::~StartStrategyResetIntentImpl4VIVO() {
    LOGV("~StartStrategyResetIntentImpl4VIVO");
}
