#include "Long.h"
#include "PthreadJNIEnvHelper.h"


bool Long::inited = false;
jclass Long::clsZ = nullptr;

void Long::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/lang/Long");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv->DeleteLocalRef(tmp);
        }
    }
}

jlong Long::longValue(const jobject& obj) {
    init();
    jlong ret = 0;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && obj != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "longValue", "()J");
        ret = jenv->CallLongMethod(obj, method);
    }

    return ret;
}
