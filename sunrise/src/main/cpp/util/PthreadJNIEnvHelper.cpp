#include <cstdio>
#include <pthread.h>
#include "PthreadJNIEnvHelper.h"
#include "Logger.h"

namespace PthreadJNIEnvHelper {
    static JavaVM * gJvm = nullptr;
    static pthread_key_t gJEnvKey;

    JavaVM* getJavaVm(){
        pthread_t thisthread = pthread_self();
        return gJvm;
    }

    void _detachCurrentThread(void* a){
        getJavaVm() ->DetachCurrentThread();
    }
    JNIEnv* _cacheEnv(JavaVM* jvm){
        JNIEnv* _env = nullptr;
        // get jni environment
        jint ret = jvm->GetEnv((void**)&_env, JNI_VERSION_1_4);

        switch (ret) {
            case JNI_OK :
                // Success!
                pthread_setspecific(gJEnvKey, _env);
                return _env;

            case JNI_EDETACHED :
                // Thread not attached
                if (jvm->AttachCurrentThread(&_env, nullptr) < 0)
                {
                    LOGE("Failed to get the environment using AttachCurrentThread()");

                    return nullptr;
                } else {
                    // Success : Attached and obtained JNIEnv!
                    pthread_setspecific(gJEnvKey, _env);
                    return _env;
                }

            case JNI_EVERSION :
                // Cannot recover from this error
                LOGE("JNI interface version 1.4 not supported");
            default :
                LOGE("Failed to get the environment using GetEnv()");
                return nullptr;
        }
    }

    void init(JavaVM *jvm) {
        pthread_t thisthread = pthread_self();

        gJvm = jvm;
        pthread_key_create(&gJEnvKey, _detachCurrentThread);
    }

    JNIEnv* getJNIEnv(){
        JNIEnv *_env = (JNIEnv *)pthread_getspecific(gJEnvKey);
        if(_env == nullptr){
            _env = _cacheEnv(gJvm);
        }

        return _env;
    }
}