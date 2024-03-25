#include "Intent.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"
#include "../util/LocalFrame.h"


bool Intent::inited = false;
jclass Intent::clsZ = nullptr;

void Intent::init() {
    if (!inited) {
        LOGV("Intent::init");

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/content/Intent");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

Intent::Intent(const jobject& core) {
    if(!inited){
        init();
    }
    LOGV("Intent::Intent, constructor");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr) {
        mCore = jenv ->NewGlobalRef(core);
    }
}

Intent::Intent(){
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        LocalFrame frame(jenv);

        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "()V");
        if(method != nullptr){
            mCore = jenv ->NewGlobalRef(jenv->NewObject(clsZ, method));
        }
    }
}

Intent::Intent(const jobject& packageContext, jclass cls) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && packageContext!= nullptr && cls!= nullptr){
        LocalFrame frame(jenv);

        jmethodID  method = jenv ->GetMethodID(clsZ, "<init>", "(Landroid/content/Context;Ljava/lang/Class;)V");
        if(method!= nullptr){
            mCore = jenv ->NewGlobalRef(jenv ->NewObject(clsZ, method, packageContext, cls));
        }
    }
}

Intent::Intent(const char *action, const jobject &uri) {
    if(!inited){
        init();
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        LocalFrame frame(jenv);

        jmethodID method = jenv ->GetMethodID(clsZ, "<init>", "(Ljava/lang/String;Landroid/net/Uri;)V");
        if(method!= nullptr){
            if(action== nullptr){
                mCore = jenv ->NewGlobalRef(jenv ->NewObject(clsZ, method, nullptr, uri));
            } else {
                mCore = jenv ->NewGlobalRef(jenv ->NewObject(clsZ, method, jenv ->NewStringUTF(action), uri));
            }
        }
    }
}


Intent& Intent::addCategory(const char *str){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr && str != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "addCategory", "(Ljava/lang/String;)Landroid/content/Intent;");

        jenv->CallObjectMethod(mCore, method,
                               jenv->NewStringUTF(str));
    }

    return *this;
}

Intent& Intent::addFlags(jint flag){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "addFlags", "(I)Landroid/content/Intent;");

        jenv->CallObjectMethod(mCore, method, flag);
    }

    return *this;
}

Intent& Intent::setPackage(jstring pkg){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr && pkg != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "setPackage", "(Ljava/lang/String;)Landroid/content/Intent;");

        jenv->CallObjectMethod(mCore, method, pkg);
    }

    return *this;
}

Intent& Intent::setComponent(const jobject& component){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = mCore;

    if(jenv != nullptr && core != nullptr && component != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "setComponent",
                                             "(Landroid/content/ComponentName;)Landroid/content/Intent;");

        jenv->CallObjectMethod(core, method, component);
    }

    return *this;
}

Intent& Intent::setAction(const char * action){
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr && action != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "setAction",
                                             "(Ljava/lang/String;)Landroid/content/Intent;");
        jenv->CallObjectMethod(mCore, method, jenv->NewStringUTF(action));
    }

    return *this;
}

Intent& Intent::putExtra(const char *name, const char* value) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();

    if(jenv!= nullptr && core!= nullptr && name!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;");

        if(value== nullptr) {
            jenv->CallObjectMethod(core, method,
                                   jenv->NewStringUTF(name),
                                   jstring(nullptr));
        } else {
            jenv->CallObjectMethod(core, method,
                                   jenv->NewStringUTF(name),
                                   jenv->NewStringUTF(value));
        }
    }

    return *this;
}

Intent& Intent::putExtra(const char *name, jint value) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr && name!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "putExtra", "(Ljava/lang/String;I)Landroid/content/Intent;");

        jenv ->CallObjectMethod(mCore, method,
                                jenv ->NewStringUTF(name),
                                value);
    }

    return *this;
}

Intent& Intent::putExtra(const char *name, jlong value) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();

    if(jenv!= nullptr && core!= nullptr && name!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "putExtra", "(Ljava/lang/String;J)Landroid/content/Intent;");

        jenv ->CallObjectMethod(core, method,
                                jenv ->NewStringUTF(name),
                                value);
    }

    return *this;
}

Intent& Intent::putExtra(const char * name, const jobject& parcelable) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr && name!= nullptr && parcelable != nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "putExtra", "(Ljava/lang/String;Landroid/os/Parcelable;)Landroid/content/Intent;");

        jenv ->CallObjectMethod(mCore, method,
                                jenv ->NewStringUTF(name),
                                parcelable);
    }

    return *this;
}

jlong Intent::getLongExtra(const char *name, jlong defVal) {
    jlong ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    jobject core = getCore();

    if(jenv!= nullptr && core!= nullptr && name!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "getLongExtra", "(Ljava/lang/String;J)J");

        ret = jenv ->CallLongMethod(core, method,
                                    jenv ->NewStringUTF(name),
                                    defVal);
    }

    return ret;
}

jint Intent::getIntExtra(const char *name, jint defVal) {
    jint ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr && name!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "getIntExtra", "(Ljava/lang/String;I)I");

        ret = jenv ->CallIntMethod(mCore, method,
                                    jenv ->NewStringUTF(name),
                                    defVal);
    }

    return ret;
}

jobject Intent::getParcelableExtra(const char* name) {
    jobject parcelable = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore!= nullptr && name!= nullptr){
        jmethodID method = jenv ->GetMethodID(clsZ, "getParcelableExtra",
                                              "(Ljava/lang/String;)Landroid/os/Parcelable;");

        parcelable = jenv ->CallObjectMethod(mCore, method,
                                    jenv ->NewStringUTF(name));
    }

    return parcelable;
}
