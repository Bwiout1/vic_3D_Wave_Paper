#ifndef AND_SHAREDPREFERENCES_H
#define AND_SHAREDPREFERENCES_H


#include "AndroidClassWrapperBase.h"


class SharedPreferences : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static jclass clsZEditor;

    static void init();

public:
    static jobject getDefaultSharedPreferences(const jobject& context);

public:
    SharedPreferences(const jobject& core);

    jboolean getBoolean(const char* key, jboolean defVal);
    jlong getLong(const char* key, jlong defVal);
    jint getInt(const char* key, jint defVal);
    jint getInt(jstring key, jint defVal);
    jstring getString(const char* key, jstring defVal);

//    SharedPreferences.Editor方法
    SharedPreferences& edit();
    SharedPreferences& putBoolean(const char* key, jboolean flag);
    SharedPreferences& putLong(const char* key, jlong value);
    SharedPreferences& putInt(const char* key, jint value);
    SharedPreferences& putInt(jstring key, jint value);
    SharedPreferences& putString(const char* key, jstring value);
    bool commit();
    void apply();

    ~SharedPreferences();
    SharedPreferences(){};
private:

    jobject mEditor = nullptr;
};


#endif //AND_SHAREDPREFERENCES_H
