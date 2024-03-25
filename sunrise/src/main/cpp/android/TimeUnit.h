#ifndef AND_TIMEUNIT_H
#define AND_TIMEUNIT_H


#include "AndroidClassWrapperBase.h"

class TimeUnit : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject SECONDS();
    static jobject MINUTES();
};


#endif //AND_TIMEUNIT_H
