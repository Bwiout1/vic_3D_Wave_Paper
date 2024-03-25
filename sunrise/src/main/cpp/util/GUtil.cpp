#include <Jni.h>
#include "PthreadJNIEnvHelper.h"


namespace GUtil {

    jclass cls_Func = nullptr;
    jmethodID mid_invoke = nullptr;

    jmethodID getFunc(int num){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv != nullptr){
            if(mid_invoke != nullptr){
                mid_invoke = nullptr;
            }
            switch (num) {
                case 0:
                    cls_Func = jenv->FindClass("kotlin/jvm/functions/Function0");
                    mid_invoke = jenv->GetMethodID(cls_Func, "invoke", "()Ljava/lang/Object;");
                    jenv->DeleteLocalRef(cls_Func);
                    return mid_invoke;
                case 1:
                    cls_Func = jenv->FindClass("kotlin/jvm/functions/Function1");
                    mid_invoke = jenv->GetMethodID(cls_Func, "invoke", "(Ljava/lang/Object;)Ljava/lang/Object;");
                    jenv->DeleteLocalRef(cls_Func);
                    return mid_invoke;
                case 2:
                    cls_Func = jenv->FindClass("kotlin/jvm/functions/Function2");
                    mid_invoke = jenv->GetMethodID(cls_Func, "invoke", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
                    jenv->DeleteLocalRef(cls_Func);
                    return mid_invoke;
                default:
                    ;
            }
        }
        return nullptr;
    }

    jmethodID getMethod(const char * cls, const char * name, const char * sig){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        jmethodID method = nullptr;
        if(jenv!= nullptr) {
            jclass clazz = jenv->FindClass(cls);
            method = jenv->GetMethodID(clazz, name, sig);
            jenv->DeleteLocalRef(clazz);
        }
        return method;
    }

    jmethodID getStaticMethod(const char * cls, const char * name, const char * sig){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass clazz = jenv->FindClass(cls);
            jmethodID mid = jenv->GetStaticMethodID(clazz, name, sig);
            jenv->DeleteLocalRef(clazz);
            if(mid != nullptr){
                return mid;
            }
        }
        return nullptr;
    }

    jobject getStaticObjectField(const char * cls, const char * name, const char * sig){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass clazz = jenv->FindClass(cls);
            jfieldID fieldId = jenv->GetStaticFieldID(clazz, name, sig);
            jobject field = jenv->GetStaticObjectField(clazz, fieldId);
            jenv->DeleteLocalRef(clazz);
            if(field != nullptr){
                return field;
            }
        }
        return nullptr;
    }

    jlong getLongField(const char * cls, const char * name, const char * sig, const jobject& obj){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        jlong num = 0;
        if(jenv!= nullptr) {
            jclass clazz = jenv->FindClass(cls);
            jfieldID fieldId = jenv->GetFieldID(clazz, name, sig);
            num = jenv->GetLongField(obj, fieldId);

            jenv->DeleteLocalRef(clazz);
        }
        return num;
    }

    jobject getObjectField(const jobject& obj, const char * cls, const char * name, const char * sig){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        jobject ob = nullptr;
        if(jenv!= nullptr) {
            jclass clazz = jenv->FindClass(cls);
            jfieldID fieldId = jenv->GetFieldID(clazz, name, sig);
            ob = jenv->GetObjectField(obj, fieldId);

            jenv->DeleteLocalRef(clazz);
        }
        return ob;
    }

    jobject callObjectMethod(const jobject& obj, const char * cls, const char * name, const char * sig){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        jobject tmp = nullptr;
        if(jenv!= nullptr) {
            jclass clazz = jenv->FindClass(cls);
            jmethodID method = jenv->GetMethodID(clazz, name, sig);
            tmp = jenv->CallObjectMethod(obj, method);

            jenv->DeleteLocalRef(clazz);
        }
        return tmp;
    }

    jobject callStaticObjectMethod(const char * cls, const char * name, const char * sig, const jobject& app){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        jobject obj = nullptr;
        if(jenv!= nullptr) {
            jclass clazz = jenv->FindClass(cls);
            jmethodID sMethod = jenv->GetStaticMethodID(clazz, name, sig);
            obj = jenv->CallStaticObjectMethod(clazz, sMethod, app);

            jenv->DeleteLocalRef(clazz);
        }
        return obj;
    }

    jobject callStaticObjectMethod(const char * cls, const char * name, const char * sig){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        jobject obj = nullptr;
        if(jenv!= nullptr) {
            jclass clazz = jenv->FindClass(cls);
            jmethodID sMethod = jenv->GetStaticMethodID(clazz, name, sig);
            obj = jenv->CallStaticObjectMethod(clazz, sMethod);

            jenv->DeleteLocalRef(clazz);
        }
        return obj;
    }
};