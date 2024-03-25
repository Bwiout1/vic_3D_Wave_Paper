#ifndef UTILS_GUTIL_H
#define UTILS_GUTIL_H


namespace GUtil {
    jmethodID getFunc(int num);
    jmethodID getMethod(const char * cls, const char * name, const char * sig);
    jmethodID getStaticMethod(const char * cls, const char * name, const char * sig);
    jobject getStaticObjectField(const char * cls, const char * name, const char * sig);
    jlong getLongField(const char * cls, const char * name, const char * sig, const jobject& obj);
    jobject getObjectField(const jobject& obj, const char * cls, const char * name, const char * sig);
    jobject callObjectMethod(const jobject& obj, const char * cls, const char * name, const char * sig);
    jobject callStaticObjectMethod(const char * cls, const char * name, const char * sig, const jobject& app);
    jobject callStaticObjectMethod(const char * cls, const char * name, const char * sig);
};


#endif //UTILS_GUTIL_H
