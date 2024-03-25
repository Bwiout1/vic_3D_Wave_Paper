#ifndef SUNRISE_SIMPLEDATEFORMAT_H
#define SUNRISE_SIMPLEDATEFORMAT_H


#include "../android/AndroidClassWrapperBase.h"
#include "Date.h"


class SimpleDateFormat: public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    SimpleDateFormat(const char* pattern, const jobject& locale);
    jstring format(Date& date);
};


#endif //SUNRISE_SIMPLEDATEFORMAT_H
