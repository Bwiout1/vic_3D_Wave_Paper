#include "OnClickListener.h"

bool OnClickListener::inited = false;
jclass OnClickListener::clsZ = nullptr;

void OnClickListener::init() {
    if (!inited) {
        LOGV("FlurryConfigListener::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/view/View$OnClickListener");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}


OnClickListener::OnClickListener() : BaseJavaInterface() {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        mCore = jenv->NewGlobalRef(generateProxy(clsZ));
    }

    onClick = nullptr;
}

void OnClickListener::overrideOnClick(std::function<void()> _onClick) {
    onClick = std::move(_onClick);
}

jobject OnClickListener::invoke(jobject method, jobjectArray objects) {
    onClick();

    return nullptr;
}