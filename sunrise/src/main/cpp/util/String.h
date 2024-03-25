#ifndef AND_STRING_H
#define AND_STRING_H


#include "../android/AndroidClassWrapperBase.h"

class String : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jboolean equals(jstring core, jobject anObject);
    static jboolean equals(jstring core, const char* str);
    static jboolean equalsIgnoreCase(jstring core, jstring anotherString);
    static jstring toLowerCase(jstring core);
    static jboolean contains(jstring core, const char* key);
    static jint length(jstring core);
    static jstring trim(jstring core);
    static jstring format(const char* format, const jobjectArray& arr);


private:
    String(){}

    String(const String&)= delete;
    String& operator=(const String&)=delete;
};


#endif //AND_STRING_H
