#ifndef SUNRISE_IRONSOURCEERROR_H
#define SUNRISE_IRONSOURCEERROR_H


#include "../../../android/AndroidClassWrapperBase.h"


class IronSourceError: public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    IronSourceError(const jobject& core);
    jstring toString();
    jint getErrorCode();

    static const jint ERROR_IS_LOAD_DURING_SHOW = 1037;
};


#endif //SUNRISE_IRONSOURCEERROR_H
