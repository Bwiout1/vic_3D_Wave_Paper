#include "StartStrategyResetIntentImpl4MIUI.h"
#include "../../util/JniException.h"

long StartStrategyResetIntentImpl4MIUI::delayTimeMills(){
    return 0;
}

const char* StartStrategyResetIntentImpl4MIUI::getName(){
    return "ResetIntentM";
}

bool StartStrategyResetIntentImpl4MIUI::start(jobject context, Intent& intent){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && intent.getCore() != nullptr){
        jclass cls = jenv->GetObjectClass(intent.getCore());
        jmethodID mid = jenv->GetMethodID(cls, "addMiuiFlags", "(I)Landroid/content/Intent;");
        if(!check_and_clear_exception(jenv)){
            jenv->CallObjectMethod(intent.getCore(), mid, 2);
            LOGV("ResetIntentM, intent.addMiuiFlags(2)");
        } else {
            LOGV("ResetIntentM, it isn't miui rom");
        }

        jenv->DeleteLocalRef(cls);
    }

    return true;
}

StartStrategyResetIntentImpl4MIUI::~StartStrategyResetIntentImpl4MIUI() {
    LOGV("~StartStrategyResetIntentImpl4MIUI");
}

