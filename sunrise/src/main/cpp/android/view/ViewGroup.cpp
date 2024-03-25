#include "ViewGroup.h"

bool ViewGroup::inited = false;
jclass ViewGroup::clsZ = nullptr;

void ViewGroup::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/view/ViewGroup");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

ViewGroup::ViewGroup(const jobject& core): View(core) {
    if(!inited){
        init();
    }
}

void ViewGroup::removeAllViews() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "removeAllViews",
                                             "()V");

        jenv->CallVoidMethod(mCore, method);
    }
}

void ViewGroup::addView(jobject const &child) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr  && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "addView",
                                             "(Landroid/view/View;)V");

        jenv->CallVoidMethod(mCore, method, child);
    }
}

jint ViewGroup::getChildCount() {
    jint ret = -1;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr  && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "getChildCount", "()I");
        ret = jenv->CallIntMethod(mCore, method);
    }

    return ret;
}
