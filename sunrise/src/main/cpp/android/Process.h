#ifndef SUNRISE_PROCESS_H
#define SUNRISE_PROCESS_H


#include "AndroidClassWrapperBase.h"


class Process : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static void killProcess(jint pid);
    static jint myPid();

};


#endif //SUNRISE_PROCESS_H
