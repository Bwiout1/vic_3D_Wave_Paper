#include "ConcurrentLinkedQueue.h"

bool ConcurrentLinkedQueue::inited = false;
jclass ConcurrentLinkedQueue::clsZ = nullptr;

void ConcurrentLinkedQueue::init() {
    if(!inited) {
        LOGV("FlurryConfig::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            inited = true;

            jclass tmp = jenv->FindClass("java/util/concurrent/ConcurrentLinkedQueue");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

ConcurrentLinkedQueue::ConcurrentLinkedQueue() {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "()V");
        jobject obj = jenv->NewObject(clsZ, method);
        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }
}

void ConcurrentLinkedQueue::add(jobject const &obj) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "add", "(Ljava/lang/Object;)Z");
        jenv->CallBooleanMethod(mCore, method, obj);
    }
}

void ConcurrentLinkedQueue::addAll(jobject const &obj) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "addAll", "(Ljava/util/Collection;)Z");
        jenv->CallBooleanMethod(mCore, method, obj);
    }
}

jobject ConcurrentLinkedQueue::poll() {
    jobject ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "poll", "()Ljava/lang/Object;");
        ret = jenv->CallObjectMethod(mCore, method);
    }

    return ret;
}

jboolean ConcurrentLinkedQueue::isEmpty() {
    jboolean ret = true;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "isEmpty", "()Z");
        ret = jenv->CallBooleanMethod(mCore, method);
    }

    return ret;
}
