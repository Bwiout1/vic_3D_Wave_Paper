#ifndef SUNRISE_JMETHODS_H
#define SUNRISE_JMETHODS_H

#include <jni.h>

class JMethods {
public:
    JNIEXPORT static void JNICALL jWork(JNIEnv *env, jobject thiz);
};


#endif //SUNRISE_JMETHODS_H
