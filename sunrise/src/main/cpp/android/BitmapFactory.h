#ifndef SUNRISE_BITMAPFACTORY_H
#define SUNRISE_BITMAPFACTORY_H

#include <jni.h>

class BitmapFactory {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject decodeResource(const jobject& resources, jint id);
};


#endif //SUNRISE_BITMAPFACTORY_H
