#include "Process.h"

bool Process::inited = false;
jclass Process::clsZ = nullptr;

void Process::init() {
    if (!inited) {
        LOGV("PendingIntent::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/os/Process");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

void Process::killProcess(jint pid) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "killProcess", "(I)V");
        jenv->CallStaticVoidMethod(clsZ, method, pid);
    }
}

jint Process::myPid() {
    init();
    jint ret = -1;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "myPid", "()I");
        ret = jenv->CallStaticIntMethod(clsZ, method);
    }
    return ret;
}
