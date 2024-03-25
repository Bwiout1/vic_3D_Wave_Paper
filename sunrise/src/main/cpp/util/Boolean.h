#ifndef SUNRISE_BOOLEAN_H
#define SUNRISE_BOOLEAN_H

#include <jni.h>


class Boolean {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jboolean booleanValue(const jobject& obj);
};


#endif //SUNRISE_BOOLEAN_H
