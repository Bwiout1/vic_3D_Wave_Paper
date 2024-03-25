#include "LayoutInflater.h"

bool LayoutInflater::inited = false;
jclass LayoutInflater::clsZ = nullptr;

void LayoutInflater::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/view/LayoutInflater");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jobject LayoutInflater::from(const jobject &context) {
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr  && context!= nullptr){
        jmethodID method = jenv->GetStaticMethodID(clsZ, "from",
                                                   "(Landroid/content/Context;)Landroid/view/LayoutInflater;");
        ret = jenv ->CallStaticObjectMethod(clsZ, method, context);
    }

    return ret;
}

LayoutInflater::LayoutInflater(const jobject& core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

jobject LayoutInflater::inflate(jint resource, ViewGroup &root, bool attachToRoot) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject v = nullptr;
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "inflate",
                                             "(ILandroid/view/ViewGroup;Z)Landroid/view/View;");

        v = jenv->CallObjectMethod(mCore, method, resource, root.getCore(), attachToRoot);
    }
    return v;
}

