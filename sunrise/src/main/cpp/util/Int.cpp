#include "Int.h"


bool Int::inited = false;
jclass Int::clsZ = nullptr;

void Int::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/lang/Integer");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv->DeleteLocalRef(tmp);
        }
    }
}

Int::Int(jint num) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "(I)V");
        jobject obj = jenv->NewObject(clsZ, method, num);

        mCore = jenv->NewGlobalRef(obj);
        jenv->DeleteLocalRef(obj);
    }
}

jboolean Int::intValue(const jobject &obj){
    init();
    jint ret = -1;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && obj != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "intValue", "()I");
        ret = jenv->CallIntMethod(obj, method);
    }

    return ret;
}
