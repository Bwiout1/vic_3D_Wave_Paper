#include "TTFeedAd.h"
#include "../../../util/List.h"

bool TTFeedAd::inited = false;
jclass TTFeedAd::clsFeedAd = nullptr;
jclass TTFeedAd::clsImage = nullptr;

void TTFeedAd::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/TTFeedAd");
            clsFeedAd = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/TTImage");
            clsImage = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

TTFeedAd::TTFeedAd(const jobject& ttAd) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && ttAd != nullptr) {
        mCore = jenv->NewGlobalRef(ttAd);
    }
}

jobject TTFeedAd::getIcon() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject obj = nullptr;
    if(jenv!= nullptr && mCore != nullptr) {
        jmethodID method = jenv->GetMethodID(clsFeedAd,
                                             "getIcon",
                                             "()Lcom/bytedance/sdk/openadsdk/TTImage;");

        obj = jenv->CallObjectMethod(getCore(), method);
    }
    return obj;
}

jobject TTFeedAd::getAdLogo() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject obj = nullptr;
    if(jenv!= nullptr && mCore != nullptr) {
        jmethodID method = jenv->GetMethodID(clsFeedAd,
                                             "getAdLogo",
                                             "()Landroid/graphics/Bitmap;");

        obj = jenv->CallObjectMethod(getCore(), method);
    }
    return obj;
}

bool TTFeedAd::isValid(jobject const &icon) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    bool flag = false;
    if(jenv != nullptr && icon != nullptr){
        jmethodID method = jenv->GetMethodID(clsImage,
                                             "isValid",
                                             "()Z");

        flag = jenv->CallBooleanMethod(icon, method);
    }
    return flag;
}

int TTFeedAd::getImageMode() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    int mode = -1;
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsFeedAd,
                                             "getImageMode",
                                             "()I");

        mode = jenv->CallIntMethod(getCore(), method);
    }
    return mode;
}

jobject TTFeedAd::getAdView() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject adView = nullptr;
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsFeedAd,
                                             "getAdView",
                                             "()Landroid/view/View;");

        adView = jenv->CallObjectMethod(getCore(), method);
    }
    return adView;
}

jobject TTFeedAd::getImageList() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject imageList = nullptr;
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsFeedAd,
                                             "getImageList",
                                             "()Ljava/util/List;");

        imageList = jenv->CallObjectMethod(getCore(), method);
    }
    return imageList;
}

void TTFeedAd::registerViewForInteraction(const jobject& viewGroup, List& list1, List& list2, const jobject& listener) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr && viewGroup != nullptr && list1.getCore() != nullptr &&
       list2.getCore() != nullptr && listener != nullptr){
        jmethodID method = jenv->GetMethodID(clsFeedAd,
                                             "registerViewForInteraction",
                                             "(Landroid/view/ViewGroup;Ljava/util/List;Ljava/util/List;Lcom/bytedance/sdk/openadsdk/TTNativeAd$AdInteractionListener;)V");

        jenv->CallVoidMethod(getCore(), method, viewGroup, list1.getCore(), list2.getCore(), listener);
    }
}

jstring TTFeedAd::getImageUrl(const jobject& obj) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject jstr = nullptr;
    if(jenv != nullptr && obj != nullptr){
        jmethodID method = jenv->GetMethodID(clsImage,
                                             "getImageUrl",
                                             "()Ljava/lang/String;");

        jstr = jenv->CallObjectMethod(obj, method);
    }
    return (jstring)jstr;
}

jstring TTFeedAd::getTitle() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject jstr = nullptr;
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsFeedAd,
                                             "getTitle",
                                             "()Ljava/lang/String;");

        jstr = jenv->CallObjectMethod(getCore(), method);
    }
    return (jstring)jstr;
}

jstring TTFeedAd::getDescription() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject jstr = nullptr;
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsFeedAd,
                                             "getDescription",
                                             "()Ljava/lang/String;");

        jstr = jenv->CallObjectMethod(getCore(), method);
    }
    return (jstring)jstr;
}

jstring TTFeedAd::getButtonText() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject jstr = nullptr;
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsFeedAd,
                                             "getButtonText",
                                             "()Ljava/lang/String;");

        jstr = jenv->CallObjectMethod(getCore(), method);
    }
    return (jstring)jstr;
}

void TTFeedAd::showPrivacyActivity() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsFeedAd, "showPrivacyActivity", "()V");
        jenv->CallVoidMethod(getCore(), method);
    }
}
