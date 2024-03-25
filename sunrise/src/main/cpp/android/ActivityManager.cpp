#include "ActivityManager.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"
#include "../util/LocalFrame.h"
#include "AndroidJni.h"

bool ActivityManager::inited = false;
jclass ActivityManager::clsZ = nullptr;

void ActivityManager::init() {
    if (!inited) {
        LOGV("ActivityManager::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/app/ActivityManager");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

ActivityManager::ActivityManager(jobject const &am) {
    if (!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && am != nullptr) {
        mCore = jenv->NewGlobalRef(am);
    }
}

jobject ActivityManager::getRunningTasks(jint maxNum) {
    jobject obj = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "getRunningTasks", "(I)Ljava/util/List;");
        obj = jenv->CallObjectMethod(getCore(), method, maxNum);
    }
    return obj;
}

void ActivityManager::moveTaskToFront(jint taskId, jint flags) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "moveTaskToFront", "(II)V");
        jenv->CallVoidMethod(getCore(), method, taskId, flags);
    }
}


jobject ActivityManager::getRunningAppProcesses() {
    jobject obj = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "getRunningAppProcesses", "()Ljava/util/List;");
        obj = jenv->CallObjectMethod(getCore(), method);
    }
    return obj;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
bool ActivityManager::RunningAppProcessInfo::inited = false;
jclass ActivityManager::RunningAppProcessInfo::clsZ = nullptr;

void ActivityManager::RunningAppProcessInfo::init() {
    if (!inited) {
        LOGV("ActivityManager::RunningAppProcessInfo::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/app/ActivityManager$RunningAppProcessInfo");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

ActivityManager::RunningAppProcessInfo::RunningAppProcessInfo(const jobject &core) {
    if (!inited) {
        init();
    }
    LOGV("ActivityManager::RunningAppProcessInfo, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);

        jfieldID field = jenv ->GetFieldID(clsZ, "processName", "Ljava/lang/String;");
        processName = static_cast<jstring>(jenv->NewGlobalRef(
                jenv->GetObjectField(mCore, field)
        ));

        field = jenv ->GetFieldID(clsZ, "importance", "I");
        importance = jenv ->GetIntField(mCore, field);

    }
}

ActivityManager::RunningAppProcessInfo::~RunningAppProcessInfo(){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv== nullptr)
        return;

    if(processName!= nullptr){
        jenv ->DeleteGlobalRef(processName);
        processName = nullptr;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
bool ActivityManager::RunningTaskInfo::inited = false;
jclass ActivityManager::RunningTaskInfo::clsZ = nullptr;

void ActivityManager::RunningTaskInfo::init() {
    if (!inited) {
        LOGV("ActivityManager_RunningTaskInfo::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/app/ActivityManager$RunningTaskInfo");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

ActivityManager::RunningTaskInfo::RunningTaskInfo(const jobject &core) {
    if (!inited) {
        init();
    }
    LOGV("ActivityManager::RunningTaskInfo, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);

        jfieldID field ;
        if(AndroidJni::get_dev_api_level()<29) {
            field = jenv->GetFieldID(clsZ, "id", "I");
        } else {
            field = jenv ->GetFieldID(clsZ, "taskId", "I");
        }
        id = jenv ->GetIntField(mCore, field);

        if(AndroidJni::get_dev_api_level()>=29){
            field = jenv ->GetFieldID(clsZ, "topActivity", "Landroid/content/ComponentName;");
            topActivity = jenv ->NewGlobalRef(
                    jenv ->GetObjectField(mCore, field)
            );
        }
    }
}

ActivityManager::RunningTaskInfo::~RunningTaskInfo(){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv== nullptr)
        return;

    if(topActivity!= nullptr){
        jenv ->DeleteGlobalRef(topActivity);
        topActivity = nullptr;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////

bool ActivityManager::TaskDescription::inited = false;
jclass ActivityManager::TaskDescription::clsZ = nullptr;

void ActivityManager::TaskDescription::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/app/ActivityManager$TaskDescription");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

ActivityManager::TaskDescription::TaskDescription(const char *str, jint id) {
    ActivityManager::TaskDescription::init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "(Ljava/lang/String;I)V");
        jstring jstr = jenv->NewStringUTF(str);
        jobject obj = jenv->NewObject(clsZ, method, jstr, id);

        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(jstr);
        jenv->DeleteLocalRef(obj);
    }
}

ActivityManager::TaskDescription::TaskDescription(const char *str, jobject const &bitmap) {
    ActivityManager::TaskDescription::init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "(Ljava/lang/String;Landroid/graphics/Bitmap;)V");
        jstring jstr = jenv->NewStringUTF(str);
        jobject obj = jenv->NewObject(clsZ, method, jstr, bitmap);

        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(jstr);
        jenv->DeleteLocalRef(obj);
    }
}

ActivityManager::TaskDescription::TaskDescription() {

}
