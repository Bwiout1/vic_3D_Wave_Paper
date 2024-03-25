#include "FacebookSdk.h"
#include "../util/PthreadJNIEnvHelper.h"

jclass FacebookSdk::clsZ = nullptr;
bool FacebookSdk::inited = false;

void FacebookSdk::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            jclass tmp = jenv->FindClass("com/facebook/FacebookSdk");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);

            inited = true;
        }
    }
}

jboolean FacebookSdk::isInitialized() {
    if(!inited) {
        init();
    }

    jboolean flag = false;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && clsZ != nullptr){
        jmethodID method = jenv->GetStaticMethodID(clsZ, "isInitialized", "()Z");
        flag = jenv->CallStaticBooleanMethod(clsZ, method);
    }

    return flag;
}

void FacebookSdk::sdkInitialize(Context &context) {
    if(!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && clsZ != nullptr){
        jmethodID method = jenv->GetStaticMethodID(clsZ, "sdkInitialize", "(Landroid/content/Context;)V");
        jenv->CallStaticVoidMethod(clsZ, method, context.getCore());
    }
}


