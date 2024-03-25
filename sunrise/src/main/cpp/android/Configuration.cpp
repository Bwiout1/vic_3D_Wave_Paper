#include "Configuration.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"

bool Configuration::inited = false;
jclass Configuration::clsZ = nullptr;

void Configuration::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/content/res/Configuration");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

Configuration::Configuration(const jobject &core) {
    if(!inited){
        init();
    }

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        mCore = jenv->NewGlobalRef(core);

        jfieldID field = jenv ->GetFieldID(clsZ, "densityDpi", "I");
        densityDpi = jenv ->GetIntField(mCore, field);

        LOGV("Configuration::densityDpi, %d", densityDpi);
    }
}