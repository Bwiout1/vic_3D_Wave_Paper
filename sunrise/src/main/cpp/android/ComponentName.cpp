#include "ComponentName.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"

bool ComponentName::inited = false;
jclass ComponentName::clsZ = nullptr;

void ComponentName::init() {
    if (!inited) {
        LOGV("ComponentName::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/content/ComponentName");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

ComponentName::ComponentName(jobject const &componentname) {
    if (!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr){
        mCore = jenv ->NewGlobalRef(componentname);
    }
}

ComponentName::ComponentName(jstring pkg, jstring cls) {
    if (!inited) {
        init();
    }
    LOGV("ComponentName::ComponentName, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
        jobject obj = jenv->NewObject(clsZ, method, pkg, cls);
        mCore = jenv ->NewGlobalRef(obj);
        jenv ->DeleteLocalRef(obj);
    }
}

ComponentName::ComponentName(Context &context, jstring cls) {
    if (!inited) {
        init();
    }
    LOGV("ComponentName::ComponentName, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "(Landroid/content/Context;Ljava/lang/String;)V");
        jobject obj = jenv->NewObject(clsZ, method, context.getCore(), cls);
        mCore = jenv ->NewGlobalRef(obj);
        jenv ->DeleteLocalRef(obj);
    }
}

jstring ComponentName::getPackageName() {
    jstring pkg = nullptr;
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "getPackageName", "()Ljava/lang/String;");
        pkg = (jstring)jenv->CallObjectMethod(getCore(), method);
    }
    return pkg;
}
