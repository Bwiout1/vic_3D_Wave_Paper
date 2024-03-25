#include "AdSlot.h"
#include "../../../util/JniException.h"

AdSlot::Builder* AdSlot::builder_native = nullptr;
AdSlot::Builder* AdSlot::builder_inter = nullptr;

AdSlot::Builder &AdSlot::newBuilder() {
    if(kind == 1){
        if(builder_native != nullptr){
            delete builder_native;
            builder_native = nullptr;
        }

        new Builder(&builder_native);
        return *builder_native;
    }else{
        if(builder_inter != nullptr){
            delete builder_inter;
            builder_inter = nullptr;
        }

        new Builder(&builder_inter);
        return *builder_inter;
    }
}

jobject AdSlot::getCore() {
    if(kind == 1){
        return builder_native->adSlot;
    }else{
        return builder_inter->adSlot;
    }
}

bool AdSlot::checkSafe() {
    if(kind == 1){
        return builder_native->isSafe;
    }else{
        return builder_inter->isSafe;
    }
}

AdSlot::AdSlot(int num) {
    kind = num;
}

/*----------------------------------------Builder-------------------------------------------------*/

bool AdSlot::Builder::inited = false;
jclass AdSlot::Builder::clsZ = nullptr;

void AdSlot::Builder::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/AdSlot$Builder");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

AdSlot::Builder::Builder(AdSlot::Builder **p) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "()V");
        jobject obj = jenv->NewObject(clsZ, method);
        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }

    *p = this;
}

AdSlot::Builder& AdSlot::Builder::setCodeId(jstring codeId){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore != nullptr && isSafe) {
        jmethodID method = jenv->GetMethodID(clsZ, "setCodeId", "(Ljava/lang/String;)Lcom/bytedance/sdk/openadsdk/AdSlot$Builder;");
        if(check_and_clear_exception(jenv)){ isSafe = false; }

        jenv->CallObjectMethod(getCore(), method, codeId);
        if(check_and_clear_exception(jenv)){ isSafe = false; }
    }

    return *this;
}

AdSlot::Builder& AdSlot::Builder::setSupportDeepLink(jboolean flag){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore != nullptr && isSafe) {
        jmethodID method = jenv->GetMethodID(clsZ, "setSupportDeepLink",
                                             "(Z)Lcom/bytedance/sdk/openadsdk/AdSlot$Builder;");
        if(check_and_clear_exception(jenv)){ isSafe = false; }

        jenv->CallObjectMethod(getCore(), method, flag);
        if(check_and_clear_exception(jenv)){ isSafe = false; }
    }

    return *this;
}

AdSlot::Builder& AdSlot::Builder::setImageAcceptedSize(jint a, jint b){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore != nullptr && isSafe) {
        jmethodID method = jenv->GetMethodID(clsZ, "setImageAcceptedSize",
                                             "(II)Lcom/bytedance/sdk/openadsdk/AdSlot$Builder;");
        if(check_and_clear_exception(jenv)){ isSafe = false; }

        jenv->CallObjectMethod(getCore(), method, a, b);
        if(check_and_clear_exception(jenv)){ isSafe = false; }
    }

    return *this;
}

AdSlot::Builder &AdSlot::Builder::build() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && mCore != nullptr && isSafe) {
        jmethodID method = jenv->GetMethodID(clsZ, "build",
                                             "()Lcom/bytedance/sdk/openadsdk/AdSlot;");
        if(check_and_clear_exception(jenv)){ isSafe = false; }

        jobject obj = jenv->CallObjectMethod(getCore(), method);
        if(check_and_clear_exception(jenv)){ isSafe = false; }

        adSlot = jenv->NewGlobalRef(obj);
        jenv->DeleteLocalRef(obj);
    }

    return *this;
}

AdSlot::Builder::~Builder() {
    if(adSlot != nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jenv->DeleteGlobalRef(adSlot);
            adSlot = nullptr;
        }
    }
}
