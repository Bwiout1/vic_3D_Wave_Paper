#include "Locale.h"

bool Locale::inited = false;
jclass Locale::clsZ = nullptr;

void Locale::init() {
    if(!inited) {

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            inited = true;

            jclass tmp = jenv->FindClass("java/util/Locale");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jobject Locale::US() {
    init();

    jobject ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jfieldID us = jenv->GetStaticFieldID(clsZ, "US", "Ljava/util/Locale;");
        ret = jenv->GetStaticObjectField(clsZ, us);
    }

    return ret;
}
