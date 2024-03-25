#ifndef SUNRISE_TTADNATIVE_H
#define SUNRISE_TTADNATIVE_H


#include "../../../android/AndroidClassWrapperBase.h"


class TTAdNative: public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    TTAdNative(const jobject& core);
    void loadFeedAd(const jobject& adSlot, jobject listener);
    void loadFullScreenVideoAd(const jobject& adSlot, jobject listener);

};


#endif //SUNRISE_TTADNATIVE_H
