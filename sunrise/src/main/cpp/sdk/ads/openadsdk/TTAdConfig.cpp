#include "TTAdConfig.h"

TTAdConfig::Builder* TTAdConfig::builder = nullptr;

TTAdConfig::Builder &TTAdConfig::newBuilder() {
    new Builder(&builder);
    return *builder;
}

jobject TTAdConfig::getCore() {
    return builder->ttAdConfig;
}


/*-----------------------------------------Builder------------------------------------------------*/

bool TTAdConfig::Builder::inited = false;
jclass TTAdConfig::Builder::clsZ = nullptr;

void TTAdConfig::Builder::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/TTAdConfig$Builder");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

TTAdConfig::Builder::Builder(Builder** p) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "()V");
        jobject obj = jenv->NewObject(clsZ, method);
        mCore = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }

    *p = this;
}

TTAdConfig::Builder &TTAdConfig::Builder::appId(jstring adId) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "appId", "(Ljava/lang/String;)Lcom/bytedance/sdk/openadsdk/TTAdConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, adId);
    }

    return *this;
}

TTAdConfig::Builder &TTAdConfig::Builder::appName(jstring name) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "appName", "(Ljava/lang/String;)Lcom/bytedance/sdk/openadsdk/TTAdConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, name);
    }

    return *this;
}

TTAdConfig::Builder &TTAdConfig::Builder::useTextureView(jboolean flag) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "useTextureView",
                                             "(Z)Lcom/bytedance/sdk/openadsdk/TTAdConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, flag);
    }

    return *this;
}

TTAdConfig::Builder &TTAdConfig::Builder::allowShowPageWhenScreenLock(jboolean flag) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "allowShowPageWhenScreenLock",
                                             "(Z)Lcom/bytedance/sdk/openadsdk/TTAdConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, flag);
    }

    return *this;
}

TTAdConfig::Builder &TTAdConfig::Builder::debug(jboolean flag) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "debug",
                                             "(Z)Lcom/bytedance/sdk/openadsdk/TTAdConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, flag);
    }

    return *this;
}

TTAdConfig::Builder &TTAdConfig::Builder::supportMultiProcess(jboolean flag) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "supportMultiProcess",
                                             "(Z)Lcom/bytedance/sdk/openadsdk/TTAdConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, flag);
    }

    return *this;
}

TTAdConfig::Builder &TTAdConfig::Builder::coppa(jint num) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "coppa",
                                             "(I)Lcom/bytedance/sdk/openadsdk/TTAdConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, num);
    }

    return *this;
}

TTAdConfig::Builder &TTAdConfig::Builder::setGDPR(jint num) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "setGDPR",
                                             "(I)Lcom/bytedance/sdk/openadsdk/TTAdConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, num);
    }

    return *this;
}

TTAdConfig::Builder &TTAdConfig::Builder::setCCPA(jint num) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "setCCPA",
                                             "(I)Lcom/bytedance/sdk/openadsdk/TTAdConfig$Builder;");
        jenv->CallObjectMethod(mCore, method, num);
    }

    return *this;
}

void TTAdConfig::Builder::build() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if (jenv != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ, "build", "()Lcom/bytedance/sdk/openadsdk/TTAdConfig;");
        jobject obj = jenv->CallObjectMethod(mCore, method);
        ttAdConfig = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }
}

TTAdConfig::Builder::~Builder() {
    if(ttAdConfig != nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if (jenv != nullptr) {
            jenv->DeleteGlobalRef(ttAdConfig);
            ttAdConfig = nullptr;
        }
    }
}
