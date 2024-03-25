#include "SimpleDateFormat.h"

bool SimpleDateFormat::inited = false;
jclass SimpleDateFormat::clsZ = nullptr;

void SimpleDateFormat::init() {
    if(!inited) {

        JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
        if(jenv!= nullptr){
            inited = true;

            jclass tmp = jenv->FindClass("java/text/SimpleDateFormat");
            clsZ = static_cast<jclass>(jenv->NewGlobalRef(tmp));
            jenv ->DeleteLocalRef(tmp);
        }
    }
}

SimpleDateFormat::SimpleDateFormat(const char *pattern, jobject const &locale) {
    init();

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "<init>", "(Ljava/lang/String;Ljava/util/Locale;)V");
        jstring jstr = jenv->NewStringUTF(pattern);
        jobject obj = jenv->NewObject(clsZ, method, jstr, locale);

        mCore = jenv->NewGlobalRef(obj);
        jenv->DeleteLocalRef(obj);
    }
}

jstring SimpleDateFormat::format(Date& date) {
    jstring ret = nullptr;

    JNIEnv *jenv = PthreadJNIEnvHelper::getJNIEnv();
    if(jenv!= nullptr){
        jmethodID method = jenv->GetMethodID(clsZ, "format", "(Ljava/util/Date;)Ljava/lang/String;");
        ret = static_cast<jstring>(jenv->CallObjectMethod(mCore, method, date.getCore()));
    }

    return ret;
}
