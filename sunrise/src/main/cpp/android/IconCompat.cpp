#include "IconCompat.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"


bool IconCompat::inited = false;
jclass IconCompat::clsZ = nullptr;

void IconCompat::init() {
    if (!inited) {

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/core/graphics/drawable/IconCompat");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}


jobject IconCompat::createWithResource(const jobject &context, jint resId) {
    if(!inited){
        init();
    }

    LOGV("IconCompat::createWithResource");
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && context!= nullptr){
        jmethodID method = jenv->GetStaticMethodID(clsZ,
                                                   "createWithResource",
                                                   "(Landroid/content/Context;I)Landroidx/core/graphics/drawable/IconCompat;");
        ret = jenv ->CallStaticObjectMethod(clsZ, method, context, resId);

    }

    return ret;
}

IconCompat::IconCompat(const jobject &core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core != nullptr){
        mCore = jenv ->NewGlobalRef(core);
    }
}
