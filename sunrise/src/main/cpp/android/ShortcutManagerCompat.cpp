#include "ShortcutManagerCompat.h"


bool ShortcutManagerCompat::inited = false;
jclass ShortcutManagerCompat::clsZ = nullptr;

void ShortcutManagerCompat::init() {
    if (!inited) {
        LOGV("ShortcutManagerCompat::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("androidx/core/content/pm/ShortcutManagerCompat");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);
        }
    }
}

jboolean ShortcutManagerCompat::isRequestPinShortcutSupported(const jobject &context) {
    if(!inited){
        init();
    }

    jboolean ret = false;

    LOGV("ShortcutManagerCompat::isRequestPinShortcutSupported");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && clsZ!= nullptr && context!= nullptr){
        jmethodID method = jenv ->GetStaticMethodID(clsZ, "isRequestPinShortcutSupported", "(Landroid/content/Context;)Z");
        ret = jenv ->CallStaticBooleanMethod(clsZ, method, context);
    }

    return ret;
}

jboolean ShortcutManagerCompat::requestPinShortcut(const jobject &context, const jobject &shortcut,
                                                   jobject callback) {
    if(!inited){
        init();
    }

    jboolean ret = false;

    LOGV("ShortcutManagerCompat::requestPinShortcut");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr && clsZ!= nullptr && context!= nullptr && shortcut!= nullptr){
        jmethodID method = jenv ->GetStaticMethodID(clsZ, "requestPinShortcut", "(Landroid/content/Context;Landroidx/core/content/pm/ShortcutInfoCompat;Landroid/content/IntentSender;)Z");
        ret = jenv ->CallStaticBooleanMethod(clsZ, method, context, shortcut, callback);
    }

    return ret;
}