#ifndef UTIL_JNIEXCEPTION_H
#define UTIL_JNIEXCEPTION_H

#include <jni.h>

extern "C"
bool check_and_clear_exception(JNIEnv *env);


#endif //UTIL_JNIEXCEPTION_H
