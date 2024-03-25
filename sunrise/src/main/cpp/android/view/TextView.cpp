#include "TextView.h"


bool TextView::inited = false;
jclass TextView::clsZ = nullptr;

void TextView::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/widget/TextView");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}


TextView::TextView(const jobject& core) : View(core) {
    if(!inited){
        init();
    }
}

void TextView::setText(jstring text) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "setText",
                                             "(Ljava/lang/CharSequence;)V");

        jenv->CallVoidMethod(mCore, method, text);
    }
}
