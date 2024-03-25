#include "ArrayList.h"


bool ArrayList::inited = false;
jclass ArrayList::clsZ = nullptr;

void ArrayList::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/util/ArrayList");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

ArrayList::ArrayList(jobject const &core) : List(core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        if(core != nullptr){
            mCore = jenv->NewGlobalRef(core);
        }else{
            jmethodID method = jenv->GetMethodID(clsZ, "<init>", "()V");
            jobject obj = jenv->NewObject(clsZ, method);
            mCore = jenv->NewGlobalRef(obj);

            jenv->DeleteLocalRef(obj);
        }
    }
}

jobject ArrayList::iterator() {
    return List::iterator();
}

void ArrayList::add(jobject const &obj) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && obj != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "add", "(Ljava/lang/Object;)Z");
        jenv->CallBooleanMethod(mCore, method, obj);
    }
}
