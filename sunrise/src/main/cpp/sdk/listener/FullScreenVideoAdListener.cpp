#include <string>
#include "FullScreenVideoAdListener.h"
#include "AdInteractionListener.h"
#include "../../util/String.h"
#include "../../util/Int.h"
#include "../../util/LocalFrame.h"
#include "../../reflect/JavaMethod.h"
#include "../ads/Pangle.h"

bool FullScreenVideoAdListener::inited = false;
jclass FullScreenVideoAdListener::clsZ = nullptr;

void FullScreenVideoAdListener::init() {
    if (!inited) {
        LOGV("AdInteractionListener::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/TTAdNative$FullScreenVideoAdListener");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

FullScreenVideoAdListener::FullScreenVideoAdListener(const char *_kind) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        kind = _kind;
        mCore = jenv->NewGlobalRef(generateProxy(clsZ));
    }
}

jobject FullScreenVideoAdListener::invoke(jobject method, jobjectArray objects) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        LocalFrame localFrame(jenv);

        jstring name = JavaMethod::getName(method);
        if (String::equals(name, "onError")) {
            auto p1 = static_cast<jstring>(jenv->GetObjectArrayElement(objects, 1));
            const char* p1_c = jenv->GetStringUTFChars(p1, nullptr);
            loge("inters failed:%d - %s", Int::intValue(jenv->GetObjectArrayElement(objects, 0)), p1_c);

            if(std::string(kind) == "loadInters"){
                Pangle::getInstance().aInters.store(false);
            }

            jenv->ReleaseStringUTFChars(p1, p1_c);
        }

        else if(String::equals(name, "onFullScreenVideoAdLoad")){
            logd("inters loaded");

            if(std::string(kind) == "loadInters"){
                Pangle::getInstance().mCacheInters->add(jenv->GetObjectArrayElement(objects, 0));
                Pangle::getInstance().aInters.store(false);
            }
        }

        else if(String::equals(name, "onFullScreenVideoCached")){
            logd("inters cached");
        }

        jenv->DeleteLocalRef(name);
    }

    return nullptr;
}
