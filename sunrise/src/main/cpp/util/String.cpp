#include "String.h"


bool String::inited = false;
jclass String::clsZ = nullptr;

void String::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/lang/String");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jboolean String::equals(jstring core, jobject anObject) {
    if(!inited)
        init();

    jboolean ret = false;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        if(core == nullptr){
            ret = (anObject== nullptr);
        } else {
            jmethodID method = jenv ->GetMethodID(clsZ, "equals", "(Ljava/lang/Object;)Z");
            ret = jenv ->CallBooleanMethod(core, method, anObject);
        }
    }

    return ret;
}

jboolean String::equals(jstring core, const char *str) {
    if(!inited)
        init();

    jboolean ret = false;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jstring jstr = jenv->NewStringUTF(str);

        jmethodID method = jenv ->GetMethodID(clsZ, "equals", "(Ljava/lang/Object;)Z");
        ret = jenv ->CallBooleanMethod(core, method, jstr);

        jenv->DeleteLocalRef(jstr);
    }

    return ret;
}

jboolean String::equalsIgnoreCase(jstring core, jstring anotherString) {
    if(!inited)
        init();

    jboolean ret = false;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        if(core == nullptr){
            ret = (anotherString== nullptr);
        } else {
            jmethodID method = jenv ->GetMethodID(clsZ, "equalsIgnoreCase", "(Ljava/lang/String;)Z");
            ret = jenv ->CallBooleanMethod(core, method, anotherString);
        }
    }

    return ret;
}

jstring String::toLowerCase(jstring core) {
    if(!inited)
        init();

    jstring ret = core;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core != nullptr){
        jclass cls_Locale = jenv->FindClass("java/util/Locale");
        jfieldID fid_root = jenv->GetStaticFieldID(cls_Locale, "ROOT", "Ljava/util/Locale;");
        jobject root = jenv->GetStaticObjectField(cls_Locale, fid_root);

        jmethodID method = jenv->GetMethodID(clsZ, "toLowerCase", "(Ljava/util/Locale;)Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->CallObjectMethod(core, method, root));

        jenv->DeleteLocalRef(cls_Locale);
        jenv->DeleteLocalRef(root);
    }

    return ret;
}

jboolean String::contains(jstring core, const char* key) {
    if(!inited)
        init();

    jboolean ret = false;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core != nullptr){
        jstring jstr = jenv->NewStringUTF(key);

        jmethodID method = jenv ->GetMethodID(clsZ, "contains", "(Ljava/lang/CharSequence;)Z");
        ret = jenv ->CallBooleanMethod(core, method, jstr);

        jenv->DeleteLocalRef(jstr);
    }

    return ret;
}

jint String::length(jstring core) {
    if(!inited)
        init();

    jint ret = 0;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core != nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "length", "()I");
        ret = jenv ->CallIntMethod(core, method);
    }

    return ret;
}

jstring String::trim(jstring core) {
    if(!inited)
        init();

    jstring ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core != nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "trim", "()Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->CallObjectMethod(core, method));
    }

    return ret;
}

jstring String::format(const char *format, const jobjectArray& arr) {
    if(!inited)
        init();

    jstring ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv->GetStaticMethodID(clsZ, "format", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;");
        jstring jstr = jenv->NewStringUTF(format);
        ret = static_cast<jstring>(jenv->CallStaticObjectMethod(clsZ, method, jstr, arr));

        jenv->DeleteLocalRef(jstr);
    }

    return ret;
}
