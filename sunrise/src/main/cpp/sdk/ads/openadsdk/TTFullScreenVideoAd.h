#ifndef SUNRISE_TTFULLSCREENVIDEOAD_H
#define SUNRISE_TTFULLSCREENVIDEOAD_H

#include "../../../android/AndroidClassWrapperBase.h"


class TTFullScreenVideoAd: public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    TTFullScreenVideoAd(const jobject& ttAd);
    void setFullScreenVideoAdInteractionListener(const jobject& listener);
    void showFullScreenVideoAd(const jobject& activity);
};


#endif //SUNRISE_TTFULLSCREENVIDEOAD_H
