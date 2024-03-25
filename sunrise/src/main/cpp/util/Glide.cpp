
#include "Glide.h"


bool Glide::inited = false;
jclass Glide::clsZ = nullptr;

void Glide::init() {
    if (!inited) {
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("com/bumptech/glide/Glide");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));

            jenv ->DeleteLocalRef(tmp);
        }
    }
}

Glide::Glide() {
    if(!inited){
        init();
    }
}

Glide & Glide::with(jobject const &context) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && context != nullptr) {
        jmethodID method = jenv->GetStaticMethodID(clsZ,
                                             "with",
                                             "(Landroid/content/Context;)Lcom/bumptech/glide/RequestManager;");

        jobject obj = jenv->CallStaticObjectMethod(clsZ, method, context);
        manager = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }
    return *this;
}

Glide & Glide::load(jstring url) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && manager != nullptr && url != nullptr) {
        jclass cls = jenv->FindClass("com/bumptech/glide/RequestManager");
        jmethodID method = jenv->GetMethodID(cls,
                                                   "load",
                                                   "(Ljava/lang/String;)Lcom/bumptech/glide/RequestBuilder;");

        jobject obj = jenv->CallObjectMethod(manager, method, url);
        builder = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(cls);
        jenv->DeleteLocalRef(obj);
    }
    return *this;
}

Glide &Glide::into(const jobject& v) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && builder != nullptr && v != nullptr) {
        jclass cls = jenv->FindClass("com/bumptech/glide/RequestBuilder");
        jmethodID method = jenv->GetMethodID(cls,
                                             "into",
                                             "(Landroid/widget/ImageView;)Lcom/bumptech/glide/request/target/ViewTarget;");

        jenv->CallObjectMethod(builder, method, v);

        jenv->DeleteLocalRef(cls);
    }
    return *this;
}

Glide::~Glide() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        if(manager != nullptr){
            jenv->DeleteGlobalRef(manager);
            manager = nullptr;
        }

        if(builder != nullptr){
            jenv->DeleteGlobalRef(builder);
            builder = nullptr;
        }
    }
}
