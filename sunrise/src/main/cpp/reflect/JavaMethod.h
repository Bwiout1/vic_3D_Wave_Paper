#ifndef SUNRISE_JAVAMETHOD_H
#define SUNRISE_JAVAMETHOD_H

#include <jni.h>


class JavaMethod {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jstring getName(const jobject & method);

};


#endif //SUNRISE_JAVAMETHOD_H
