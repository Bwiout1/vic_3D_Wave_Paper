#include "WebSettings.h"


bool WebSettings::inited = false;
jclass WebSettings::clsZ = nullptr;
jobject WebSettings::LAYOUT_ALGORITHM_NORMAL = nullptr;

void WebSettings::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/webkit/WebSettings");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);

            tmp = jenv ->FindClass("android/webkit/WebSettings$LayoutAlgorithm");
            jfieldID field = jenv ->GetStaticFieldID(tmp, "NORMAL", "Landroid/webkit/WebSettings$LayoutAlgorithm;");
            LAYOUT_ALGORITHM_NORMAL = jenv ->NewGlobalRef(
                    jenv ->GetStaticObjectField(tmp, field)
                    );
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

WebSettings::WebSettings(const jobject &core) {
    if(!inited){
        init();
    }

    JNIEnv* jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        mCore = jenv->NewGlobalRef(core);
    }
}

void WebSettings::setJavaScriptEnabled(jboolean flag) {
    LOGV("WebSettings::setJavaScriptEnabled...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setJavaScriptEnabled", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, flag);
    }
}

void WebSettings::setUseWideViewPort(jboolean use) {
    LOGV("WebSettings::setUseWideViewPort...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setUseWideViewPort", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, use);
    }
}

void WebSettings::setLoadWithOverviewMode(jboolean overview) {
    LOGV("WebSettings::setLoadWithOverviewMode...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setLoadWithOverviewMode", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, overview);
    }
}

void WebSettings::setSupportZoom(jboolean support) {
    LOGV("WebSettings::setSupportZoom...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setSupportZoom", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, support);
    }
}

void WebSettings::setBuiltInZoomControls(jboolean enabled) {
    LOGV("WebSettings::setBuiltInZoomControls...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setBuiltInZoomControls", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, enabled);
    }
}

void WebSettings::setMediaPlaybackRequiresUserGesture(jboolean require) {
    LOGV("WebSettings::setMediaPlaybackRequiresUserGesture...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setMediaPlaybackRequiresUserGesture", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, require);
    }
}

void WebSettings::setDisplayZoomControls(jboolean enabled) {
    LOGV("WebSettings::setDisplayZoomControls...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setDisplayZoomControls", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, enabled);
    }
}

void WebSettings::setDomStorageEnabled(jboolean flag) {
    LOGV("WebSettings::setDomStorageEnabled...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setDomStorageEnabled", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, flag);
    }
}

void WebSettings::setGeolocationEnabled(jboolean flag) {
    LOGV("WebSettings::setGeolocationEnabled...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setGeolocationEnabled", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, flag);
    }
}

void WebSettings::setJavaScriptCanOpenWindowsAutomatically(jboolean flag) {
    LOGV("WebSettings::setJavaScriptCanOpenWindowsAutomatically...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setJavaScriptCanOpenWindowsAutomatically", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, flag);
    }
}

void WebSettings::setNeedInitialFocus(jboolean flag) {
    LOGV("WebSettings::setNeedInitialFocus...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setNeedInitialFocus", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, flag);
    }
}

void WebSettings::setSaveFormData(jboolean save) {
    LOGV("WebSettings::setSaveFormData...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setSaveFormData", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, save);
    }
}

void WebSettings::setSupportMultipleWindows(jboolean support) {
    LOGV("WebSettings::setSupportMultipleWindows...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setSupportMultipleWindows", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, support);
    }
}

void WebSettings::setLayoutAlgorithm(const jobject l) {
    LOGV("WebSettings::setLayoutAlgorithm...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setLayoutAlgorithm",
                                              "(Landroid/webkit/WebSettings$LayoutAlgorithm;)V");
        jenv ->CallVoidMethod(getCore(), method, l);
    }
}

void WebSettings::setAppCachePath(jstring path) {
    LOGV("WebSettings::setAppCachePath...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setAppCachePath", "(Ljava/lang/String;)V");
        jenv ->CallVoidMethod(getCore(), method, path);
    }
}

void WebSettings::setAppCacheEnabled(jboolean enabled) {
    LOGV("WebSettings::setAppCacheEnabled...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setAppCacheEnabled", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, enabled);
    }
}

void WebSettings::setDatabaseEnabled(jboolean enabled) {
    LOGV("WebSettings::setAppCacheEnabled...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setDatabaseEnabled", "(Z)V");
        jenv ->CallVoidMethod(getCore(), method, enabled);
    }
}

void WebSettings::setCacheMode(jint mode) {
    LOGV("WebSettings::setCacheMode...");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "setCacheMode", "(I)V");
        jenv ->CallVoidMethod(getCore(), method, mode);
    }
}