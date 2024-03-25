#ifndef REFLECT_INVOCATIONHANDLERIMP_H
#define REFLECT_INVOCATIONHANDLERIMP_H


#include "../android/AndroidClassWrapperBase.h"
#include "Proxy.h"
#include "JavaClass.h"
#include "../util/PthreadJNIEnvHelper.h"

class BaseJavaInterface;


class NativeInvocationHandler : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;


    static void init();

public:
    static jobject JNICALL nativeInvoke(JNIEnv *env, jclass clazz, jlong jniObj, jobject o,
                                        jobject method, jobjectArray params);

public:
    NativeInvocationHandler(BaseJavaInterface *ptr);

private:
    NativeInvocationHandler() {
        if(!inited){
            init();
        }
    }

    NativeInvocationHandler(const NativeInvocationHandler&) = delete;
    NativeInvocationHandler& operator=(const NativeInvocationHandler&)=delete;
};


#endif //REFLECT_INVOCATIONHANDLERIMP_H
