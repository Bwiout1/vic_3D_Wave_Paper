#include "NotificationChannel.h"
#include "AndroidJni.h"

bool NotificationChannel::inited = false;
jclass NotificationChannel::clsZ = nullptr;

void NotificationChannel::init() {
    if(AndroidJni::get_dev_api_level() <26)
        return;

    if (!inited) {
        LOGV("NotificationChannel::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/app/NotificationChannel");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

NotificationChannel::NotificationChannel(const jobject &core) {
    if(AndroidJni::get_dev_api_level() <26)
        return;

    if (!inited) {
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && clsZ != nullptr){
        mCore = jenv->NewGlobalRef(core);
    }
}

NotificationChannel::NotificationChannel(const char *id, const char *name, jint importance) {
    if(AndroidJni::get_dev_api_level() <26)
        return;

    LOGV("NotificationChannel::NotificationChannel(id, name, importance)");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && clsZ != nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "<init>", "(Ljava/lang/String;Ljava/lang/CharSequence;I)V");
        mCore = jenv ->NewGlobalRef(
                jenv ->NewObject(clsZ, method, jenv ->NewStringUTF(id), jenv ->NewStringUTF(name), importance)
                );
    }
}

void NotificationChannel::enableLights(jboolean lights) {
    if(AndroidJni::get_dev_api_level() <26)
        return;

    LOGV("NotificationChannel::enableLights(%d)", lights);

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "enableLights", "(Z)V");
        jenv ->CallVoidMethod(mCore, method, lights);
    }
}

void NotificationChannel::setShowBadge(jboolean showBadge) {
    if(AndroidJni::get_dev_api_level() <26)
        return;

    LOGV("NotificationChannel::setShowBadge(%d)", showBadge);

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setShowBadge", "(Z)V");
        jenv ->CallVoidMethod(mCore, method, showBadge);
    }
}

void NotificationChannel::enableVibration(jboolean vibration) {
    if(AndroidJni::get_dev_api_level() <26)
        return;

    LOGV("NotificationChannel::enableVibration(%d)", vibration);

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "enableVibration", "(Z)V");
        jenv ->CallVoidMethod(mCore, method, vibration);
    }
}

void NotificationChannel::setSound(jobject sound, jobject audioAttributes) {
    if(AndroidJni::get_dev_api_level() <26)
        return;

    LOGV("NotificationChannel::setSound");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setSound",
                                              "(Landroid/net/Uri;Landroid/media/AudioAttributes;)V");
        jenv ->CallVoidMethod(mCore, method, sound, audioAttributes);
    }
}