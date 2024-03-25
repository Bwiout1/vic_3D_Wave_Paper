#include "Presentation.h"
#include "../util/Logger.h"
#include "../util/PthreadJNIEnvHelper.h"


bool Presentation::inited = false;
jclass Presentation::clsZ = nullptr;

void Presentation::init() {
    if (!inited) {
        LOGV("Presentation::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/app/Presentation");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

Presentation::Presentation(const jobject &core) {
    if(!inited){
        init();
    }

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        mCore = jenv->NewGlobalRef(core);
    }
}

Presentation::Presentation(const jobject &outerContext, const jobject &display) {
    if(!inited){
        init();
    }

    LOGV("Presentation::Presentation(const jobject &outerContext, const jobject &display)");

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID constructor = jenv ->GetMethodID(clsZ, "<init>", "(Landroid/content/Context;Landroid/view/Display;)V");
        mCore = jenv ->NewGlobalRef(
                jenv ->NewObject(clsZ, constructor, outerContext, display)
        );
    }
}

void Presentation::show() {
    LOGV("Presentation::show");

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "show", "()V");
        jenv ->CallVoidMethod(mCore, method);
    }
}