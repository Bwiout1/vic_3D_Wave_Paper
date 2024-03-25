#include "ShortcutInfoCompat_Builder.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"

bool ShortcutInfoCompat_Builder::inited = false;
jclass ShortcutInfoCompat_Builder::clsZ = nullptr;

void ShortcutInfoCompat_Builder::init() {
    if (!inited) {
        LOGV("ShortcutInfoCompat_Builder::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/core/content/pm/ShortcutInfoCompat$Builder");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);
        }
    }
}

ShortcutInfoCompat_Builder::ShortcutInfoCompat_Builder(const jobject &context, jstring id) {
    if(!inited){
        init();
    }

    LOGV("ShortcutInfoCompat_Builder::ShortcutInfoCompat_Builder, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && context!= nullptr) {
        jmethodID constructor = jenv ->GetMethodID(clsZ, "<init>", "(Landroid/content/Context;Ljava/lang/String;)V");
        jobject localRef = jenv ->NewObject(clsZ, constructor, context, id);

        mCore = jenv ->NewGlobalRef(localRef);
        jenv->DeleteLocalRef(localRef);
    }
}

ShortcutInfoCompat_Builder &ShortcutInfoCompat_Builder::setIcon(const jobject& icon) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    if (jenv != nullptr && mCore!= nullptr && icon!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setIcon", "(Landroidx/core/graphics/drawable/IconCompat;)Landroidx/core/content/pm/ShortcutInfoCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, icon);
    }

    return *this;
}

ShortcutInfoCompat_Builder& ShortcutInfoCompat_Builder::setShortLabel(jstring shortLabel) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    if (jenv != nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setShortLabel", "(Ljava/lang/CharSequence;)Landroidx/core/content/pm/ShortcutInfoCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, shortLabel);
    }

    return *this;
}


ShortcutInfoCompat_Builder& ShortcutInfoCompat_Builder::setLongLabel(jstring longLabel) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    if (jenv != nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setLongLabel", "(Ljava/lang/CharSequence;)Landroidx/core/content/pm/ShortcutInfoCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, longLabel);
    }

    return *this;
}

ShortcutInfoCompat_Builder& ShortcutInfoCompat_Builder::setIntent(const jobject &intent) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    if(jenv!= nullptr && mCore!= nullptr && intent!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setIntent", "(Landroid/content/Intent;)Landroidx/core/content/pm/ShortcutInfoCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, intent);
    }

    return *this;
}

ShortcutInfoCompat_Builder& ShortcutInfoCompat_Builder::setActivity(const jobject &activity) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    if(jenv!= nullptr && mCore!= nullptr && activity!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setActivity", "(Landroid/content/ComponentName;)Landroidx/core/content/pm/ShortcutInfoCompat$Builder;");
        jenv ->CallObjectMethod(mCore, method, activity);
    }

    return *this;
}

jobject ShortcutInfoCompat_Builder::build() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "build", "()Landroidx/core/content/pm/ShortcutInfoCompat;");
        ret = jenv ->CallObjectMethod(mCore, method);
    }

    return ret;
}