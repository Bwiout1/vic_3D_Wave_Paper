#include "Context.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"
#include "../util/LocalFrame.h"
#include "PackageManager.h"

bool Context::inited = false;
jclass Context::contextClsZ = nullptr;
jclass Context::activityClsZ = nullptr;

void Context::init() {
    if (!inited) {
        LOGV("Context::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/content/Context");
            contextClsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);

            tmp = jenv->FindClass("android/app/Activity");
            activityClsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jclass Context::getContextClass() {
    if (!inited) {
        init();
    }

    return contextClsZ;
}

jclass Context::getActivityClass() {
    if (!inited) {
        init();
    }

    return activityClsZ;
}


Context::Context(const jobject &core) {
    if (!inited) {
        init();
    }
    LOGV("Context::Context, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

void Context::startActivity(const jobject& intent){
    LOGV("Context::startActivity");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();
    if(jenv!= nullptr && core!= nullptr && intent!= nullptr){
        jmethodID method = jenv ->GetMethodID(contextClsZ, "startActivity", "(Landroid/content/Intent;)V");
        jenv ->CallVoidMethod(core, method, intent);
    }
}

jobject Context::startForegroundService(const jobject& intent){
    jobject ret = nullptr;
    LOGV("Context::startForegroundService");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();
    if(jenv!= nullptr && core!= nullptr && intent!= nullptr){
        jmethodID method = jenv ->GetMethodID(contextClsZ, "startForegroundService",
                                              "(Landroid/content/Intent;)Landroid/content/ComponentName;");
        ret = jenv ->CallObjectMethod(core, method, intent);
    }

    return ret ;
}

jobject Context::getSystemService(const char * name){
    jobject ret = nullptr;
    LOGV("Context::getSystemService");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();
    if(jenv!= nullptr && core!= nullptr && name!= nullptr){
        jmethodID method = jenv ->GetMethodID(contextClsZ, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
        ret = jenv ->CallObjectMethod(core, method, jenv ->NewStringUTF(name));
    }

    return ret;
}

jobject Context::getPackageManager() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {

        jclass clzWrapper ;

        // 这里从android/app/Activity取jmethodID而没有从android/content/ContextWrapper或者android/app/Application里取
        // 是用来对抗AndroidNativeEmu框架
        if(activityClsZ!= nullptr)
            clzWrapper = activityClsZ;
        else
            clzWrapper = contextClsZ;

        if(clzWrapper!= nullptr){
            jmethodID method = jenv ->GetMethodID(clzWrapper, "getPackageManager", "()Landroid/content/pm/PackageManager;");
            ret = jenv ->CallObjectMethod(getCore(), method);
        }
    }

    return ret;
}

jstring Context::getPackageName() {
    jstring ret = nullptr;

    if(contextClsZ!= nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr && mCore!= nullptr){
            jmethodID method = jenv ->GetMethodID(contextClsZ, "getPackageName","()Ljava/lang/String;");
            if(method!= nullptr){
                ret = static_cast<jstring>(jenv->CallObjectMethod(getCore(), method));
            }
        }
    }

    if(ret== nullptr){
        LOGW("can't get package name");
    }

    return ret;
}

jobject Context::getResources() {
    jobject ret = nullptr;
    LOGV("Context::getResources");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();
    if(jenv!= nullptr && core!= nullptr){
        jmethodID method = jenv ->GetMethodID(contextClsZ, "getResources", "()Landroid/content/res/Resources;");
        ret = jenv ->CallObjectMethod(core, method);
    }

    return ret;
}


jstring Context::getProcessName() {
    jstring name = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && getCore() != nullptr){
        jmethodID method = jenv->GetMethodID(contextClsZ, "getApplicationInfo", "()Landroid/content/pm/ApplicationInfo;");
        jclass cls = jenv->FindClass("android/content/pm/ApplicationInfo");
        jfieldID fid = jenv->GetFieldID(cls, "processName", "Ljava/lang/String;");

        jobject obj = jenv->CallObjectMethod(getCore(), method);
        name = (jstring)jenv->GetObjectField(obj, fid);

        jenv->DeleteLocalRef(cls);
        jenv->DeleteLocalRef(obj);
    }
    return name;
}

jstring Context::getAppName() {
    jstring name = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && getCore() != nullptr){
        PackageManager packageManager(getPackageManager());
        jobject packageInfo = packageManager.getPackageInfo(getPackageName(), 0);
        jobject applicationInfo = packageManager.applicationInfo(packageInfo);
        name = static_cast<jstring>(packageManager.getApplicationLabel(applicationInfo));

        jenv->DeleteLocalRef(packageInfo);
        jenv->DeleteLocalRef(applicationInfo);
    }
    return name;
}
