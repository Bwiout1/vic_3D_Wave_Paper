#include <jni.h>
#include <cassert>
#include "util/Logger.h"
#include "util/PthreadJNIEnvHelper.h"
#include "android/AndroidJni.h"
#include "guard/Guard.h"
#include "sdk/JMethods.h"
#include "util/LocalFrame.h"
#include "util/JniException.h"
#include "sdk/bgactivity/StartStrategyList.h"
#include "sdk/SunSdk.h"
#include "sdk/plans/SunActivity.h"


bool registerJavaMethod(JNIEnv *env){
    if(env== nullptr)
        return false;
    LocalFrame frame(env);

    jclass jEntryClz = env ->FindClass("com/sunrise/sunrise/SunSpot");
    if(check_and_clear_exception(env)){
        return false ;
    }

    const JNINativeMethod Methods[] = {
            {"work", "()V", (void*)JMethods::jWork},
            {"notify", "(Landroid/content/Intent;)V", (void*)StartStrategyList::notify},
            {"post", "([Ljava/lang/Object;)V", (void*)SunSdk::post},
            {"onCreate", "([Ljava/lang/Object;)V", (void*)SunActivity::onCreate},
    };

    env ->RegisterNatives(jEntryClz, Methods, sizeof(Methods) / sizeof (Methods[0]));
    if(check_and_clear_exception(env))
        return false;

    return true;
}

JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGV("开始动态注册JNI");
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        return -1;
    }

    PthreadJNIEnvHelper::init(vm);

    assert(env != nullptr);
    if(registerJavaMethod(env) && AndroidJni::registerJavaMethod(env)){
        LOGD("注册JNI完成");
    }else{
        LOGE("注册JNI出错");
    }

/**
 * 开启反调试监测线程
 */
//     todo: after updating md5 of apk sign key, enable this checking
//    Guard::startGuardThread();

    return JNI_VERSION_1_6;
}