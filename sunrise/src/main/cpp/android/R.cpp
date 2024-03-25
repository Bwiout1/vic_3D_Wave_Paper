
#include "R.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/JniException.h"

jint R::getLayoutId(const char* name) {
    jint id = -1;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jclass cls = jenv->FindClass("com/xmas/icontest/R$layout");
        if(!check_and_clear_exception(jenv)){
            jfieldID fid = jenv->GetStaticFieldID(cls, name, "I");
            if(!check_and_clear_exception(jenv)){
                id = jenv->GetStaticIntField(cls, fid);
            }
        }
        jenv->DeleteLocalRef(cls);
    }
    return id;
}

jint R::getDrawableId(const char* name) {
    jint id = -1;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jclass cls = jenv->FindClass("com/xmas/icontest/R$drawable");
        if(!check_and_clear_exception(jenv)){
            jfieldID fid = jenv->GetStaticFieldID(cls, name, "I");
            if(!check_and_clear_exception(jenv)){
                id = jenv->GetStaticIntField(cls, fid);
            }
        }
        jenv->DeleteLocalRef(cls);
    }
    return id;
}

jint R::getMipmapId(const char* name) {
    jint id = -1;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jclass cls = jenv->FindClass("com/xmas/icontest/R$mipmap");
        if(!check_and_clear_exception(jenv)){
            jfieldID fid = jenv->GetStaticFieldID(cls, name, "I");
            if(!check_and_clear_exception(jenv)){
                id = jenv->GetStaticIntField(cls, fid);
            }
        }
        jenv->DeleteLocalRef(cls);
    }
    return id;
}
