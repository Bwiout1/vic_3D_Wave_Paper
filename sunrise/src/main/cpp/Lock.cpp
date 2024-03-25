#include <jni.h>
#include <sys/file.h>
#include <unistd.h>
#include "util/Logger.h"


char * getProcessName(JNIEnv *env){
    jclass cls_Application = env->FindClass("android/app/Application");
    //9.0以上
    jclass cls_Build_VERSION = env->FindClass("android/os/Build$VERSION");
    jfieldID sfid_SDK_INT = env->GetStaticFieldID(cls_Build_VERSION, "SDK_INT", "I");
    int SDK_INT = env->GetStaticIntField(cls_Build_VERSION, sfid_SDK_INT);
    if(SDK_INT >= 28){
        jmethodID mid_getProcessName = env->GetStaticMethodID(cls_Application, "getProcessName", "()Ljava/lang/String;");
        auto ss = (jstring)env->CallStaticObjectMethod(cls_Application, mid_getProcessName);

        env->DeleteLocalRef(cls_Application);
        env->DeleteLocalRef(cls_Build_VERSION);

        return (char *)env->GetStringUTFChars(ss, nullptr);
    }else{
        env->DeleteLocalRef(cls_Application);
        env->DeleteLocalRef(cls_Build_VERSION);

        return (char *)"i dont know";
    }

}


int lockDescriptor;

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_sunrise_sunrise_MyLibrary_lLock(JNIEnv *env, jobject thiz, jstring str_lock) {
    char *lock_file_path = (char *) env->GetStringUTFChars(str_lock, nullptr);

    //只读
    lockDescriptor = open(lock_file_path, O_RDONLY);
    if (lockDescriptor == -1) {
        //若不存在,创建可读的新文件
        lockDescriptor = open(lock_file_path, O_CREAT, S_IRUSR);
    }
    LOGD("lockFileDescriptor1:%d, process:%s", lockDescriptor, getProcessName(env));
    LOGD("try to lock lockRes file, process:%s", getProcessName(env));


    //flock:建议性锁,参考 https://zhuanlan.zhihu.com/p/25134841
    //对文件建立互斥锁定
    //非阻塞flock,lock失败将立刻返回-1
    int lockRet = flock(lockDescriptor, LOCK_EX | LOCK_NB);
    //如果建立互斥失败,返回-1;成功返回0
    if (lockRet == -1) {
        LOGD("im waiting for decode res..., process:%s", getProcessName(env));
        flock(lockDescriptor, LOCK_SH);
        flock(lockDescriptor, LOCK_UN);
        LOGD("waiting completed!, process:%s", getProcessName(env));
        return false;
    } else {
        LOGD("lock lockRes success. path:%s, process:%s", lock_file_path, getProcessName(env));
        return true;
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_sunrise_sunrise_MyLibrary_lUnlock(JNIEnv *env, jobject thiz, jstring str_lock) {
    char *lock_file_path = (char *) env->GetStringUTFChars(str_lock, nullptr);

    LOGD("lockFileDescriptor2:%d, process:%s", lockDescriptor, getProcessName(env));
    if (lockDescriptor == -1) {
        //若不存在
        LOGE("unlock lockRes error, %s is not exist", lock_file_path);
    }else{
        int unLockRet = flock(lockDescriptor, LOCK_UN);
        if(unLockRet == -1){
            LOGE("unlock lockRes error, flock fail, process:%s", getProcessName(env));
        }else if(unLockRet == 0){
            LOGD("unlock lockRes succeed, process:%s", getProcessName(env));
        }

        int closeRet = close(lockDescriptor);
        if(!closeRet){
            LOGD("close lockFile succeed");
        }else{
            LOGE("close lockFile fail");
        }
    }
}