#ifndef UTIL_PTHREADJNIENVHELPER_H
#define UTIL_PTHREADJNIENVHELPER_H
#include <jni.h>

/**
 * 用法：
 * a.每个进程调用一次init(JavaVM*)
 * b.每个pthread线程要使用JNIEnv时，调用getJNIEnv()
 * 当线程退出时候，自动清理JNIEnv
 */
namespace PthreadJNIEnvHelper{
    void init(JavaVM* jvm);
    JavaVM* getJavaVm();
    JNIEnv* getJNIEnv();
}

#endif //UTIL_PTHREADJNIENVHELPER_H
