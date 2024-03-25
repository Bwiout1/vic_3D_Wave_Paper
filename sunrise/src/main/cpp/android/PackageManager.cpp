#include "PackageManager.h"
#include "../util/Logger.h"
#include "../util/PthreadJNIEnvHelper.h"

bool PackageManager::inited = false;
jclass PackageManager::clsZ = nullptr;

void PackageManager::init() {
    if (!inited) {
        LOGV("PackageManager::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/content/pm/PackageManager");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jclass PackageManager::getClass() {
    if (!inited) {
        init();
    }

    return clsZ;
}

PackageManager::PackageManager(const jobject &core) {
    if (!inited) {
        init();
    }

    LOGV("PackageManager::PackageManager, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

jobject PackageManager::getPackageInfo(jstring packageName, jint flags) {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
        ret = jenv ->CallObjectMethod(getCore(), method, packageName, flags);
    }

    return ret;
}

jint PackageManager::getComponentEnabledSetting(jobject const &componentName) {
    jint num = -1;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "getComponentEnabledSetting",
                                             "(Landroid/content/ComponentName;)I");

        num = jenv->CallIntMethod(getCore(), method, componentName);
    }
    return num;
}

void PackageManager::setComponentEnabledSetting(const jobject& componentName, jint newState, jint flags) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "setComponentEnabledSetting",
                                             "(Landroid/content/ComponentName;II)V");

        jenv->CallVoidMethod(getCore(), method, componentName, newState, flags);
    }
}

jobject PackageManager::queryIntentActivities(jobject const &obj, jint num) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject ob = nullptr;
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "queryIntentActivities", "(Landroid/content/Intent;I)Ljava/util/List;");
        ob = jenv->CallObjectMethod(getCore(), method, obj, num);
    }
    return ob;
}

jobject PackageManager::applicationInfo(jobject const &packageInfo) {
    jobject ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && packageInfo!= nullptr){
        jclass cls = jenv->FindClass("android/content/pm/PackageInfo");
        jfieldID fid = jenv->GetFieldID(cls, "applicationInfo", "Landroid/content/pm/ApplicationInfo;");

        ret = jenv->GetObjectField(packageInfo, fid);
        jenv->DeleteLocalRef(cls);
    }

    return ret;
}

jobject PackageManager::getApplicationLabel(jobject const &applicationInfo) {
    jobject ret = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr && applicationInfo!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "getApplicationLabel", "(Landroid/content/pm/ApplicationInfo;)Ljava/lang/CharSequence;");
        ret = jenv->CallObjectMethod(getCore(), method, applicationInfo);
    }

    return ret;
}

jboolean PackageManager::checkPermission(const char *permName, jstring packageName) {
    jboolean ret = false;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr){
        jstring jstr = jenv->NewStringUTF(permName);
        jmethodID method = jenv->GetMethodID(clsZ, "checkPermission", "(Ljava/lang/String;Ljava/lang/String;)I");

        ret = PERMISSION_GRANTED == jenv->CallIntMethod(getCore(), method, jstr, packageName);
    }

    return ret;
}
