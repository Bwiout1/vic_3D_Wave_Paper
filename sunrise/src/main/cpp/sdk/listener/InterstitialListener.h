#ifndef SUNRISE_INTERSTITIALLISTENER_H
#define SUNRISE_INTERSTITIALLISTENER_H

#include "../../reflect/BaseJavaInterface.h"


class InterstitialListener: public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

    const char* kind = "";

public:
    InterstitialListener(const char* _kind);
    jobject invoke(jobject method, jobjectArray objects);

private:
    InterstitialListener(const InterstitialListener&)=delete;
    InterstitialListener& operator=(const InterstitialListener&)=delete;

};


#endif //SUNRISE_INTERSTITIALLISTENER_H
