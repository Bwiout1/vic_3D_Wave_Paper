#include "MediationAdapterUtil.h"

bool MediationAdapterUtil::inited = false;
jclass MediationAdapterUtil::clsZ = nullptr;

void MediationAdapterUtil::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/adapter/MediationAdapterUtil");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

void MediationAdapterUtil::addNativeFeedMainView(jobject const &context, jint i,
                                                 jobject const &mediaView, jobject const &view,
                                                 jobject const &list) {

    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ, "addNativeFeedMainView", "(Landroid/content/Context;ILcom/bytedance/sdk/openadsdk/adapter/MediaView;Landroid/view/View;Ljava/util/List;)V");
        jenv->CallStaticVoidMethod(clsZ, method, context, i, mediaView, view, list);
    }
}

