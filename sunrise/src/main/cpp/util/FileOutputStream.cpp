#include "FileOutputStream.h"
#include "../util/PthreadJNIEnvHelper.h"

bool FileOutputStream::inited = false;
jclass FileOutputStream::clsZ = nullptr;

void FileOutputStream::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("java/io/FileOutputStream");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}


FileOutputStream::FileOutputStream(jstring name) {
    if(!inited){
        init();
    }
    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && name!= nullptr){
        jmethodID constructor = jenv ->GetMethodID(clsZ, "<init>","(Ljava/lang/String;)V");
        if(constructor!= nullptr){
            jobject instFOS = jenv ->NewObject(clsZ, constructor, name);
            mCore = jenv->NewGlobalRef(instFOS);
            jenv ->DeleteLocalRef(instFOS);
        }
    }
}

void FileOutputStream::write(jbyteArray data){
    if(clsZ!= nullptr && mCore != nullptr) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID write = jenv ->GetMethodID(clsZ, "write", "([B)V");
            if(write!= nullptr){
                jenv ->CallVoidMethod(mCore, write, data);
            }
        }
    }
}


void FileOutputStream::flush() {
    if(clsZ!= nullptr && mCore != nullptr) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID flush = jenv ->GetMethodID(clsZ, "flush", "()V");
            if(flush!= nullptr){
                jenv ->CallVoidMethod(mCore, flush);
            }
        }
    }
}

void FileOutputStream::close() {
    if(clsZ!= nullptr && mCore != nullptr) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jmethodID close = jenv ->GetMethodID(clsZ, "close", "()V");
            if(close!= nullptr){
                return jenv ->CallVoidMethod(mCore, close);
            }
        }
    }
}
