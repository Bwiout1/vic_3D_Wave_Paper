#include "NativeInvocationHandler.h"
#include "../android/Handler.h"
#include "BaseJavaInterface.h"

bool NativeInvocationHandler::inited = false;
jclass NativeInvocationHandler::clsZ = nullptr;

void NativeInvocationHandler::init() {
    if (!inited) {
        LOGV("NativeInvocationHandler::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/lang/InvocationHandlerImp");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);
        }
    }
}

NativeInvocationHandler::NativeInvocationHandler(BaseJavaInterface* ptr) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LOGE("NativeInvocationHandler-ptr:%ld", (jlong)ptr);
        jmethodID constructor = jenv->GetMethodID(clsZ, "<init>", "(J)V");
        jobject obj = jenv->NewObject(clsZ, constructor, (jlong)ptr);
        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }
}

jobject NativeInvocationHandler::nativeInvoke(JNIEnv *env, jclass clazz, jlong jniObj, jobject o, jobject method,
                                           jobjectArray params) {
    jobject ret = nullptr;

    LOGE("NativeInvocationHandler-jniObj:%ld", jniObj);
    auto *p = (BaseJavaInterface *)jniObj;
    if(p != nullptr ){
        if(p->getMagicNum() == BaseJavaInterface::MAGIC){
            ret = p->invoke(method, params);
        } else {
            LOGE("It's not a native BaseJavaInterface obj, please check it,mMagicNum is %ld, MAGIC is %ld",
                 p->getMagicNum(),
                 BaseJavaInterface::MAGIC);
        }

    }

    return ret;
}

