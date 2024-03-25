#ifndef BGACTIVITY_INTENTUTILS_H
#define BGACTIVITY_INTENTUTILS_H


#include <jni.h>

class IntentUtils {
public:
    static jboolean b(const jobject& context_, const jobject& intent_, jboolean isPending);
    static void moveAppToFront(const jobject& context_);
    static jboolean isAppInForeground(const jobject& context_);

    static jboolean JNICALL lb(JNIEnv *env, jclass clazz, jobject context, jobject intent,
                               jboolean isPending);
};


#endif //BGACTIVITY_INTENTUTILS_H
