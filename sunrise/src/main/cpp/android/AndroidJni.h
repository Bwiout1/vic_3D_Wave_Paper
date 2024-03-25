#ifndef AND_ANDROID_JNI_H
#define AND_ANDROID_JNI_H

#include <jni.h>
#include "Handler.h"

namespace AndroidJni {
    bool registerJavaMethod(JNIEnv *env);

    Handler& getUIHandler();

    jint get_dev_api_level();

    jboolean IsInstanceOf(jobject jobj, jclass clz);
}


#endif //AND_ANDROID_JNI_H
