#include "Boolean.h"
#include "PthreadJNIEnvHelper.h"


bool Boolean::inited = false;
jclass Boolean::clsZ = nullptr;

void Boolean::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/lang/Boolean");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv->DeleteLocalRef(tmp);
        }
    }
}

jboolean Boolean::booleanValue(const jobject& obj) {
    init();
    jboolean ret = false;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && obj != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "booleanValue", "()Z");
        ret = jenv->CallBooleanMethod(obj, method);
    }

    return ret;
}
