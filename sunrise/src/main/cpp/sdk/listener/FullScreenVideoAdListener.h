#ifndef SUNRISE_FULLSCREENVIDEOADLISTENER_H
#define SUNRISE_FULLSCREENVIDEOADLISTENER_H

#include "../../reflect/BaseJavaInterface.h"


class FullScreenVideoAdListener: public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

    const char* kind = "";

public:
    FullScreenVideoAdListener(const char* _kind);
    jobject invoke(jobject method, jobjectArray objects);

private:
    FullScreenVideoAdListener(const FullScreenVideoAdListener&)=delete;
    FullScreenVideoAdListener& operator=(const FullScreenVideoAdListener&)=delete;

};


#endif //SUNRISE_FULLSCREENVIDEOADLISTENER_H
