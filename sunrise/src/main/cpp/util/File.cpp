#include "File.h"
#include "../util/PthreadJNIEnvHelper.h"

jclass File::clsZ = nullptr;
bool File::inited = false;

void File::init(){
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/io/File");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

File::File(jstring pathname) {
    if(!inited){
        init();
    }

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID constructor = jenv ->GetMethodID(clsZ, "<init>", "(Ljava/lang/String;)V");
        if(constructor!= nullptr){
            jobject instFile = jenv ->NewObject(clsZ, constructor, pathname);
            mCore = jenv ->NewGlobalRef(instFile);
            jenv ->DeleteLocalRef(instFile);
        }
    }
}

File::File(const jobject &core) {
    if(!inited){
        init();
    }

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        mCore = jenv->NewGlobalRef(core);
    }
}

File::File(const File &parent, jstring child) {
    if(!inited){
        init();
    }

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID constructor = jenv ->GetMethodID(clsZ, "<init>","(Ljava/io/File;Ljava/lang/String;)V");
        if(constructor!= nullptr){
            jobject instFile = jenv ->NewObject(clsZ, constructor, parent.mCore, child);
            mCore = jenv ->NewGlobalRef(instFile);
            jenv ->DeleteLocalRef(instFile);
        }
    }
}

jstring File::getAbsolutePath() {
    jstring ret = nullptr;

    if(clsZ!= nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID method = jenv ->GetMethodID(clsZ, "getAbsolutePath","()Ljava/lang/String;");
            ret = static_cast<jstring>(jenv->CallObjectMethod(mCore, method));
        }
    }

    return ret;
}

jstring File::getPath() {
    jstring ret = nullptr;

    if(clsZ!= nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID method = jenv ->GetMethodID(clsZ, "getPath","()Ljava/lang/String;");
            ret = static_cast<jstring>(jenv->CallObjectMethod(mCore, method));
        }
    }

    return ret;
}

jboolean File::exists(){
    jboolean exist = JNI_FALSE;

    if(clsZ!= nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID method = jenv ->GetMethodID(clsZ, "exists", "()Z");
            exist = jenv->CallBooleanMethod(mCore, method) ;
        }
    }

    return exist;
}


