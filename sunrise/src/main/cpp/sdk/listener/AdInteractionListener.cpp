#include <string>
#include "AdInteractionListener.h"
#include "../../util/String.h"
#include "../../util/HashMap.h"
#include "../../reflect/JavaMethod.h"
#include "../control/Reporter.h"

bool AdInteractionListener::inited = false;
jclass AdInteractionListener::clsZ = nullptr;

void AdInteractionListener::init() {
    if (!inited) {
        LOGV("AdInteractionListener::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bytedance/sdk/openadsdk/TTAdNative$FeedAdListener");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}


AdInteractionListener::AdInteractionListener(const char* _kind) : BaseJavaInterface() {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        kind = _kind;
        mCore = jenv->NewGlobalRef(generateProxy(clsZ));
    }
}

jobject AdInteractionListener::invoke(jobject method, jobjectArray objects) {

    jstring name = JavaMethod::getName(method);

    if(String::equals(name, "onAdClicked")){
        logd("native onClick");
    }

    else if(String::equals(name, "onAdCreativeClick")){
        logd("native creativeClick");
    }

    else if(String::equals(name, "onAdShow")){
        logd("native onShow");

        if(std::string(kind) == "fillCleanNative"){
            HashMap hashMap = HashMap();
            Reporter::report("clean_native", hashMap);
        }
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        jenv->DeleteLocalRef(name);
    }


    return nullptr;
}
