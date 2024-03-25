#include "JniException.h"
#include "Logger.h"

bool check_and_clear_exception(JNIEnv *env){
    if(env== nullptr)
        return true;

    if(env ->ExceptionCheck()){
#ifndef NDEBUG
        env ->ExceptionDescribe();
#endif
        env ->ExceptionClear();

        return true;
    }

    return false;
}
