#include "VirtualDisplay.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"

bool VirtualDisplay::inited = false;
jclass VirtualDisplay::clsZ = nullptr;

void VirtualDisplay::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/hardware/display/VirtualDisplay");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

VirtualDisplay::VirtualDisplay(const jobject &core) {
    if(!inited){
        init();
    }

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        mCore = jenv->NewGlobalRef(core);
    }
}

jobject VirtualDisplay::getDisplay() {
    jobject ret = nullptr;

    LOGV("VirtualDisplay::getDisplay...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "getDisplay", "()Landroid/view/Display;");
        ret = jenv ->CallObjectMethod(mCore, method);
    }

    return ret;
}