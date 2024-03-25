#include "HashMap.h"


bool HashMap::inited = false;
jclass HashMap::clsZ = nullptr;

void HashMap::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/util/HashMap");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

HashMap::HashMap() {
    if (!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "()V");
        jobject obj = jenv->NewObject(clsZ, method);
        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }
}

HashMap &HashMap::put(jobject const &key, jobject const &val) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        jenv->CallObjectMethod(mCore, method, key, val);

        if(isNull){
            isNull = false;
        }
    }

    return *this;
}

HashMap &HashMap::put(const char *key, jobject const &val) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jstring jstr = jenv->NewStringUTF(key);
        jmethodID method = jenv->GetMethodID(clsZ, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        jenv->CallObjectMethod(mCore, method, jstr, val);

        if(isNull){
            isNull = false;
        }

        jenv->DeleteLocalRef(jstr);
    }

    return *this;
}

HashMap &HashMap::put(const char *str1, const char *str2) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jstring jstr1 = jenv->NewStringUTF(str1);
        jstring jstr2 = jenv->NewStringUTF(str2);
        jmethodID method = jenv->GetMethodID(clsZ, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        jenv->CallObjectMethod(mCore, method, jstr1, jstr2);

        if(isNull){
            isNull = false;
        }

        jenv->DeleteLocalRef(jstr1);
        jenv->DeleteLocalRef(jstr2);
    }

    return *this;
}
