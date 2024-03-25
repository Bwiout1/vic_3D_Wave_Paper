#include "View.h"

bool View::inited = false;
jclass View::clsZ = nullptr;

void View::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/view/View");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

View::View(const jobject& core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core!= nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

jobject View::findViewById(jint id) {
    jobject v = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "findViewById",
                                             "(I)Landroid/view/View;");

        v = jenv->CallObjectMethod(mCore, method, id);
    }
    return v;
}

jobject View::getContext() {
    jobject context = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "getContext", "()Landroid/content/Context;");
        context = jenv->CallObjectMethod(mCore, method);
    }

    return context;
}

jobject View::getParent() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "getParent", "()Landroid/view/ViewParent;");
        ret = jenv->CallObjectMethod(mCore, method);
    }

    return ret;
}

jobject View::getTag() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "getTag", "()Ljava/lang/Object;");
        ret = jenv->CallObjectMethod(mCore, method);
    }

    return ret;
}

void View::setVisibility(jint visibility) {

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "setVisibility", "(I)V");
        jenv->CallVoidMethod(mCore, method, visibility);
    }
}

void View::setOnClickListener(jobject l) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "setOnClickListener", "(Landroid/view/View$OnClickListener;)V");
        jenv->CallVoidMethod(mCore, method, l);
    }
}
