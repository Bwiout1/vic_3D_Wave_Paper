#ifndef SUNRISE_JSONOBJECT_H
#define SUNRISE_JSONOBJECT_H


#include "../android/AndroidClassWrapperBase.h"


class JSONObject: public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    JSONObject(jstring core);
    jboolean has(const char* str);
    jstring getString(const char* key);
};


#endif //SUNRISE_JSONOBJECT_H
