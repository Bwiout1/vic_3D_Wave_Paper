#include "NotificationManagerCompat.h"


bool NotificationManagerCompat::inited = false;
jclass NotificationManagerCompat::clsZ = nullptr;

void NotificationManagerCompat::init() {
    if (!inited) {
        LOGV("NotificationManagerCompat::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/core/app/NotificationManagerCompat");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jobject NotificationManagerCompat::from(jobject context) {
    if (!inited) {
        init();
    }

    LOGV("NotificationManagerCompat::from(context)");
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && context!= nullptr){
        jmethodID method = jenv ->GetStaticMethodID(clsZ, "from", "(Landroid/content/Context;)Landroidx/core/app/NotificationManagerCompat;");
        ret = jenv ->CallStaticObjectMethod(clsZ, method, context);
    }

    return ret;
}

NotificationManagerCompat::NotificationManagerCompat(const jobject& core) {
    if (!inited) {
        init();
    }
    LOGV("NotificationManagerCompat::NotificationManagerCompat, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

void NotificationManagerCompat::notify(jint id, jobject notification) {
    LOGV("NotificationManagerCompat::notify(id, notification)");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr  && mCore!= nullptr  &&  notification!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "notify", "(ILandroid/app/Notification;)V");

        jenv ->CallVoidMethod(mCore, method, id, notification);
    }
}

void NotificationManagerCompat::cancel(jint id) {
    LOGV("NotificationManagerCompat::cancle(id)");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr  && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "cancel", "(I)V");

        jenv ->CallVoidMethod(mCore, method, id);
    }
}

void NotificationManagerCompat::createNotificationChannel(jobject channel) {
    LOGV("NotificationManagerCompat::createNotificationChannel");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr  && mCore!= nullptr  && channel!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "createNotificationChannel", "(Landroid/app/NotificationChannel;)V");

        jenv ->CallVoidMethod(mCore, method, channel);
    }
}