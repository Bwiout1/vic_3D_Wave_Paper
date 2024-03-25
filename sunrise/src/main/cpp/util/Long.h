#ifndef SUNRISE_LONG_H
#define SUNRISE_LONG_H

#include <jni.h>


class Long {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jlong longValue(const jobject& obj);
};


#endif //SUNRISE_LONG_H
