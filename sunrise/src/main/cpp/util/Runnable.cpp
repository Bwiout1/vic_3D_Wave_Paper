#include <cstdlib>
#include "Runnable.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"
#include "../util/String.h"
#include "../android/Handler.h"
#include "../android/AndroidJni.h"
#include "../reflect/JavaMethod.h"
#include "../util/Long.h"

bool Runnable::inited = false;
jclass Runnable::clsZ = nullptr;


void Runnable::init() {
    if (!inited) {
        LOGV("Runnable::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/lang/Runnable");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }

        LOGV("Runnable magic num:%ld", MAGIC);
    }
}

Runnable::Runnable( RunnableCallback callback, void *arg)
        : BaseJavaInterface() {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        mCore = jenv->NewGlobalRef(generateProxy(clsZ));

        mCallback = callback;
        mArg = arg;
    }
}

jobject Runnable::invoke(jobject method, jobjectArray objects) {
    mCallback(mArg);

    return nullptr;
}
