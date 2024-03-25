#include "SharedPreferences.h"
#include "../util/PthreadJNIEnvHelper.h"
#include "../util/Logger.h"


bool SharedPreferences::inited = false;
jclass SharedPreferences::clsZ = nullptr;
jclass SharedPreferences::clsZEditor = nullptr;

void SharedPreferences::init() {
    if (!inited) {

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr) {
            inited = true;

            jclass tmp = jenv->FindClass("android/content/SharedPreferences");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);

            tmp = jenv->FindClass("android/content/SharedPreferences$Editor");
            clsZEditor = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}



jobject SharedPreferences::getDefaultSharedPreferences(jobject const &context) {
    if(!inited){
        init() ;
    }

    jobject ret = nullptr;
    LOGV("PreferenceManager::getDefaultSharedPreferences");

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && context!= nullptr) {
        jclass cls = jenv->FindClass("androidx/preference/PreferenceManager");
        jmethodID method = jenv->GetStaticMethodID(cls,
                                                   "getDefaultSharedPreferences",
                                                   "(Landroid/content/Context;)Landroid/content/SharedPreferences;");

        ret = jenv->CallStaticObjectMethod(cls, method, context);

        jenv->DeleteLocalRef(cls);
    }
    return ret;
}

SharedPreferences::SharedPreferences(const jobject& core){
    if(!inited){
        init() ;
    }

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && core!= nullptr){
        mCore = jenv ->NewGlobalRef(core);
    }
}

jboolean SharedPreferences::getBoolean(const char* key, jboolean defVal) {
    jboolean ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ,
                                                   "getBoolean",
                                                   "(Ljava/lang/String;Z)Z");

        ret = jenv->CallBooleanMethod(mCore, method, jenv->NewStringUTF(key), defVal);
    }

    return ret;
}

jlong SharedPreferences::getLong(const char *key, jlong defVal) {
    jlong ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr) {
        jstring jstr = jenv->NewStringUTF(key);
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "getLong",
                                             "(Ljava/lang/String;J)J");

        ret = jenv->CallLongMethod(mCore, method, jstr, defVal);

        jenv->DeleteLocalRef(jstr);
    }

    return ret;
}

jint SharedPreferences::getInt(const char *key, jint defVal) {
    jint ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr) {
        jstring jstr = jenv->NewStringUTF(key);
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "getInt",
                                             "(Ljava/lang/String;I)I");

        ret = jenv->CallIntMethod(mCore, method, jstr, defVal);

        jenv->DeleteLocalRef(jstr);
    }

    return ret;
}

jint SharedPreferences::getInt(jstring key, jint defVal) {
    jint ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr) {
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "getInt",
                                             "(Ljava/lang/String;I)I");

        ret = jenv->CallIntMethod(mCore, method, key, defVal);
    }

    return ret;
}

jstring SharedPreferences::getString(const char *key, jstring defVal) {
    jstring ret = defVal;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr && mCore != nullptr) {
        jstring jstr = jenv->NewStringUTF(key);
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "getString",
                                             "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

        ret = static_cast<jstring>(jenv->CallObjectMethod(mCore, method, jstr, defVal));

        jenv->DeleteLocalRef(jstr);
    }

    return ret;
}

SharedPreferences &SharedPreferences::edit() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mCore != nullptr){
        jmethodID method = jenv->GetMethodID(clsZ,
                                             "edit",
                                             "()Landroid/content/SharedPreferences$Editor;");

        jobject obj = jenv->CallObjectMethod(mCore, method);

        if(mEditor!= nullptr){
            jenv ->DeleteGlobalRef(mEditor);
            mEditor = nullptr;
        }
        mEditor = jenv->NewGlobalRef(obj);

        jenv->DeleteLocalRef(obj);
    }
    return *this;
}

SharedPreferences &SharedPreferences::putBoolean(const char *key, jboolean flag) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mEditor != nullptr){
        jstring jstr = jenv->NewStringUTF(key);
        jmethodID method = jenv->GetMethodID(clsZEditor,
                                             "putBoolean",
                                             "(Ljava/lang/String;Z)Landroid/content/SharedPreferences$Editor;");

        jobject localRef = jenv->CallObjectMethod(mEditor, method, jstr, flag);
        jenv ->DeleteGlobalRef(mEditor);
        mEditor = jenv ->NewGlobalRef(localRef);

        jenv->DeleteLocalRef(localRef);
        jenv->DeleteLocalRef(jstr);
    }
    return *this;
}

SharedPreferences &SharedPreferences::putLong(const char *key, jlong value) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mEditor != nullptr){
        jstring jstr = jenv->NewStringUTF(key);

        jmethodID method = jenv->GetMethodID(clsZEditor,
                                             "putLong",
                                             "(Ljava/lang/String;J)Landroid/content/SharedPreferences$Editor;");

        jobject localRef = jenv->CallObjectMethod(mEditor, method, jstr, value);
        jenv ->DeleteGlobalRef(mEditor);
        mEditor = jenv ->NewGlobalRef(localRef);

        jenv->DeleteLocalRef(localRef);
        jenv->DeleteLocalRef(jstr);
    }
    return *this;
}

SharedPreferences &SharedPreferences::putInt(const char *key, jint value) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mEditor != nullptr){
        jstring jstr = jenv->NewStringUTF(key);

        jmethodID method = jenv->GetMethodID(clsZEditor,
                                             "putInt",
                                             "(Ljava/lang/String;I)Landroid/content/SharedPreferences$Editor;");

        jobject localRef = jenv->CallObjectMethod(mEditor, method, jstr, value);
        jenv ->DeleteGlobalRef(mEditor);
        mEditor = jenv ->NewGlobalRef(localRef);

        jenv->DeleteLocalRef(localRef);
        jenv->DeleteLocalRef(jstr);
    }
    return *this;
}

SharedPreferences &SharedPreferences::putInt(jstring key, jint value) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mEditor != nullptr){

        jmethodID method = jenv->GetMethodID(clsZEditor,
                                             "putInt",
                                             "(Ljava/lang/String;I)Landroid/content/SharedPreferences$Editor;");

        jobject localRef = jenv->CallObjectMethod(mEditor, method, key, value);
        jenv ->DeleteGlobalRef(mEditor);
        mEditor = jenv ->NewGlobalRef(localRef);

        jenv->DeleteLocalRef(localRef);
    }
    return *this;
}

SharedPreferences &SharedPreferences::putString(const char *key, jstring value) {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mEditor != nullptr){
        jstring jstr = jenv->NewStringUTF(key);

        jmethodID method = jenv->GetMethodID(clsZEditor,
                                             "putString",
                                             "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");


        jobject localRef = jenv->CallObjectMethod(mEditor, method, jstr, value);
        jenv ->DeleteGlobalRef(mEditor);
        mEditor = jenv ->NewGlobalRef(localRef);

        jenv->DeleteLocalRef(localRef);
        jenv->DeleteLocalRef(jstr);
    }
    return *this;
}

bool SharedPreferences::commit() {
    bool ret = false;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv != nullptr && mEditor != nullptr){
        jmethodID method = jenv->GetMethodID(clsZEditor, "commit", "()Z");
        ret = jenv->CallBooleanMethod(mEditor, method);

        jenv->DeleteGlobalRef(mEditor);
        mEditor = nullptr;
    }

    return ret;
}

void SharedPreferences::apply() {
    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();

    if(jenv != nullptr && mEditor != nullptr){
        jmethodID method = jenv->GetMethodID(clsZEditor, "apply", "()V");
        jenv->CallVoidMethod(mEditor, method);

        jenv->DeleteGlobalRef(mEditor);
        mEditor = nullptr;
    }
}

SharedPreferences::~SharedPreferences() {
    if(mEditor != nullptr){
        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv != nullptr){
            jenv->DeleteGlobalRef(mEditor);
            mEditor = nullptr;
        }
    }
}
