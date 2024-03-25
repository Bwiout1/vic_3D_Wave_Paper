#include "WebView.h"

bool WebView::inited = false;
jclass WebView::clsZ = nullptr;

void WebView::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/webkit/WebView");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

jobject WebView::constructor(const jobject &context) {
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && context!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "<init>", "(Landroid/content/Context;)V");
        ret = jenv ->NewObject(clsZ, method, context);
    }

    return ret;
}

void WebView::setBackgroundColor(const jobject &core, jint color) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "setBackgroundColor", "(I)V");
        jenv ->CallVoidMethod(core, method, color);
    }
}

jobject WebView::getSettings(const jobject &core) {
    if(!inited){
        init();
    }

    jobject ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "getSettings", "()Landroid/webkit/WebSettings;");
        ret = jenv ->CallObjectMethod(core, method);
    }

    return ret;
}


WebView::WebView(const jobject &core) : ViewGroup(core) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core != nullptr) {
        mCore = jenv->NewGlobalRef(core);
    }
}

WebView::WebView(Context &context) : ViewGroup(nullptr) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jobject core = constructor(context.getCore());
        if (core != nullptr) {
            mCore = jenv->NewGlobalRef(core);
        }
    }
}

void WebView::setBackgroundColor(jint color) {
    setBackgroundColor(mCore, color);
}

jobject WebView::getSettings() {
    return getSettings(mCore);
}

void WebView::loadUrl(jstring url) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr && url != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "loadUrl", "(Ljava/lang/String;)V");
        jenv->CallVoidMethod(mCore, method, url);
    }
}
