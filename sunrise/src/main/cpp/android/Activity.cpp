#include "Activity.h"


bool Activity::inited = false;
jclass Activity::clsZ = nullptr;

void Activity::init() {
    if(!inited){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            jclass tmp = jenv->FindClass("android/app/Activity");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv->DeleteLocalRef(tmp);

            inited = true;
        }
    }
}

Activity::Activity(jobject const &context) : Context(context){
    if(!inited) {
        init();
    }

    LOGV("Activity::Activity, constructor");
}

jboolean Activity::isDestroyed() {
    jboolean ret = true;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "isDestroyed", "()Z");
        ret = jenv->CallBooleanMethod(mCore, method);
    }

    return ret;
}

jboolean Activity::isFinishing() {
    jboolean ret = true;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "isFinishing", "()Z");
        ret = jenv->CallBooleanMethod(mCore, method);
    }

    return ret;
}

jobject Activity::getResources() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method  = jenv->GetMethodID(clsZ, "getResources", "()Landroid/content/res/Resources;");
        ret = jenv->CallObjectMethod(mCore, method);
    }

    return ret;
}

void Activity::setTaskDescription(jobject const &description) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "setTaskDescription", "(Landroid/app/ActivityManager$TaskDescription;)V");
        jenv->CallVoidMethod(mCore, method, description);
    }
}

jobject Activity::findViewWithTag(const char *str) {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jobject window = getWindow();

        jclass cls_Window = jenv->FindClass("android/view/Window");
        jmethodID getDecorView = jenv->GetMethodID(cls_Window, "getDecorView",
                                                   "()Landroid/view/View;");
        jobject view = jenv->CallObjectMethod(window, getDecorView);

        jclass cls_View = jenv->FindClass("android/view/View");
        jmethodID findViewWithTag = jenv->GetMethodID(cls_View, "findViewWithTag", "(Ljava/lang/Object;)Landroid/view/View;");
        jstring jstr = jenv->NewStringUTF(str);

        ret = jenv->CallObjectMethod(view, findViewWithTag, jstr);

        jenv->DeleteLocalRef(window);
        jenv->DeleteLocalRef(cls_Window);
        jenv->DeleteLocalRef(view);
        jenv->DeleteLocalRef(cls_View);
        jenv->DeleteLocalRef(jstr);
    }

    return ret;
}

jobject Activity::getIntent() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "getIntent", "()Landroid/content/Intent;");
        ret = jenv->CallObjectMethod(getCore(), method);
    }

    return ret;
}

jobject Activity::getWindow() {
    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "getWindow", "()Landroid/view/Window;");
        ret = jenv->CallObjectMethod(getCore(), method);
    }

    return ret;
}
