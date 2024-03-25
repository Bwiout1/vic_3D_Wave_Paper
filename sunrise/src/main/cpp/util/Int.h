#ifndef SUNRISE_INT_H
#define SUNRISE_INT_H

#include "../android/AndroidClassWrapperBase.h"

class Int : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Int(jint num);
    static jboolean intValue(const jobject& obj);

};


#endif //SUNRISE_INT_H
