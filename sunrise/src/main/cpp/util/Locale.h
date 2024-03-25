#ifndef SUNRISE_LOCALE_H
#define SUNRISE_LOCALE_H


#include "../android/AndroidClassWrapperBase.h"


class Locale: public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    static jobject US();

};


#endif //SUNRISE_LOCALE_H
