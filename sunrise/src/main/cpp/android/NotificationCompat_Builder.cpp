#include "NotificationCompat_Builder.h"


bool NotificationCompat_Builder::inited = false;
jclass NotificationCompat_Builder::clsZ = nullptr;

void NotificationCompat_Builder::init() {
    if (!inited) {
        LOGV("NotificationCompat_Builder::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/core/app/NotificationCompat$Builder");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

NotificationCompat_Builder::NotificationCompat_Builder(const jobject &core) {
    if (!inited) {
        init();
    }
    LOGV("NotificationCompat_Builder::NotificationCompat_Builder(core), constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

NotificationCompat_Builder::NotificationCompat_Builder(jobject context, const char *channelId) {
    if (!inited) {
        init();
    }
    LOGV("NotificationCompat_Builder::NotificationCompat_Builder(context, channelId), constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && context!= nullptr && channelId!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "<init>", "(Landroid/content/Context;Ljava/lang/String;)V");

        mCore = jenv ->NewGlobalRef(
                jenv ->NewObject(clsZ, method, context, jenv ->NewStringUTF(channelId))
                );
    }
}

NotificationCompat_Builder& NotificationCompat_Builder::setContentTitle(const char *title) {
    LOGV("NotificationCompat_Builder::setContentTitle(title)");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setContentTitle", "(Ljava/lang/CharSequence;)Landroidx/core/app/NotificationCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, title== nullptr ? (jobject)nullptr : jenv ->NewStringUTF(title));
    }

    return *this;
}

NotificationCompat_Builder& NotificationCompat_Builder::setContentText(const char *text) {
    LOGV("NotificationCompat_Builder::setContentText(text)");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setContentText", "(Ljava/lang/CharSequence;)Landroidx/core/app/NotificationCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, text== nullptr ? (jobject)nullptr : jenv ->NewStringUTF(text));
    }

    return *this;
}

NotificationCompat_Builder& NotificationCompat_Builder::setSound(jobject sound) {
    LOGV("NotificationCompat_Builder::setSound(sound)");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setSound", "(Landroid/net/Uri;)Landroidx/core/app/NotificationCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, sound);
    }

    return *this;
}

NotificationCompat_Builder& NotificationCompat_Builder::setSmallIcon(jint icon) {
    LOGV("NotificationCompat_Builder::setSmallIcon");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setSmallIcon", "(I)Landroidx/core/app/NotificationCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, icon);
    }

    return *this;
}

NotificationCompat_Builder& NotificationCompat_Builder::setPriority(jint pri) {
    LOGV("NotificationCompat_Builder::setPriority");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setPriority", "(I)Landroidx/core/app/NotificationCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, pri);
    }

    return *this;
}

NotificationCompat_Builder& NotificationCompat_Builder::setCategory(const char *category) {
    LOGV("NotificationCompat_Builder::setCategory(category)");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setCategory", "(Ljava/lang/String;)Landroidx/core/app/NotificationCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, category== nullptr ? (jobject)nullptr : jenv ->NewStringUTF(category));
    }

    return *this;
}

NotificationCompat_Builder& NotificationCompat_Builder::setCustomContentView(jobject contentView) {
    LOGV("NotificationCompat_Builder::setCustomContentView");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID  method = jenv ->GetMethodID(clsZ, "setCustomContentView",
                                               "(Landroid/widget/RemoteViews;)Landroidx/core/app/NotificationCompat$Builder;");

        jenv ->CallObjectMethod(mCore, method, contentView);
    }

    return *this;
}

NotificationCompat_Builder& NotificationCompat_Builder::setFullScreenIntent(jobject intent,
                                                                            jboolean highPriority) {
    LOGV("NotificationCompat_Builder::setFullScreenIntent");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID  method = jenv ->GetMethodID(clsZ, "setFullScreenIntent",
                                               "(Landroid/app/PendingIntent;Z)Landroidx/core/app/NotificationCompat$Builder;");

        jenv ->CallObjectMethod(mCore, method, intent, highPriority);
    }

    return *this;
}

jobject NotificationCompat_Builder::build() {
    jobject ret = nullptr;

    LOGV("NotificationCompat_Builder::build");
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "build", "()Landroid/app/Notification;");
        ret = jenv ->CallObjectMethod(mCore, method);
    }

    return ret;
}
