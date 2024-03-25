#ifndef SUNRISE_MEDIATIONADAPTERUTIL_H
#define SUNRISE_MEDIATIONADAPTERUTIL_H

#include "../../../android/AndroidClassWrapperBase.h"


class MediationAdapterUtil: public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static void addNativeFeedMainView(const jobject& context, jint i, const jobject& mediaView, const jobject& view, const jobject& list);

};


#endif //SUNRISE_MEDIATIONADAPTERUTIL_H
