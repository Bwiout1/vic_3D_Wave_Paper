#include "RemoteViews.h"

bool RemoteViews::inited = false;
jclass RemoteViews::clsZ = nullptr;

void RemoteViews::init() {
    if (!inited) {
        LOGV("RemoteViews::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/widget/RemoteViews");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

RemoteViews::RemoteViews(jstring packageName, jint layoutId) {
    if (!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "<init>", "(Ljava/lang/String;I)V");
        mCore = jenv ->NewGlobalRef(
                jenv ->NewObject(clsZ, method, packageName, layoutId)
                );
    }
}