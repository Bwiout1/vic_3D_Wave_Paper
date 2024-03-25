#ifndef SUNRISE_JSONARRAY_H
#define SUNRISE_JSONARRAY_H


#include "../android/AndroidClassWrapperBase.h"


class JSONArray: public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    JSONArray(jstring str);
    jint length();
    jstring optString(jint num);

};


#endif //SUNRISE_JSONARRAY_H
