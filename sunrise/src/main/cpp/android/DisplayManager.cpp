#include "DisplayManager.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"


bool DisplayManager::inited = false;
jclass DisplayManager::clsZ = nullptr;

void DisplayManager::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/hardware/display/DisplayManager");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

DisplayManager::DisplayManager(const jobject &core) {
    if(!inited){
        init();
    }

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        mCore = jenv->NewGlobalRef(core);
    }
}

jobject DisplayManager::createVirtualDisplay(const char *name, jint width, jint height,
                                             jint densityDpi, jobject surface, jint flags) {
    jobject ret = nullptr;
    LOGV("DisplayManager::createVirtualDisplay...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr && name!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "createVirtualDisplay", "(Ljava/lang/String;IIILandroid/view/Surface;I)Landroid/hardware/display/VirtualDisplay;");
        ret = jenv ->CallObjectMethod(mCore, method, jenv ->NewStringUTF(name), width, height, densityDpi, surface, flags);
    }

    return ret;
}