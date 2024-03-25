#include "Proxy.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "JavaClass.h"

bool Proxy::inited = false;
jclass Proxy::clsZ = nullptr;

void Proxy::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("java/lang/reflect/Proxy");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }

        inited = true;
    }
}

jobject Proxy::newProxyInstance(jobject loader, jobject interfaces, jobject h) {
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && loader != nullptr  &&  interfaces!= nullptr && h!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ,
                                                   "newProxyInstance",
                                                   "(Ljava/lang/ClassLoader;[Ljava/lang/Class;Ljava/lang/reflect/InvocationHandler;)Ljava/lang/Object;");

        ret = jenv->CallStaticObjectMethod(clsZ, method, loader, interfaces, h);

    }

    return ret;
}


