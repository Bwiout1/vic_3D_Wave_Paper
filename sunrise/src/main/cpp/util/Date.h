#ifndef SUNRISE_DATE_H
#define SUNRISE_DATE_H


#include "../android/AndroidClassWrapperBase.h"


class Date: public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    Date(jlong time);
    Date();
};


#endif //SUNRISE_DATE_H
