#ifndef SUNRISE_REPORTER_H
#define SUNRISE_REPORTER_H

#include <jni.h>
#include "../../util/HashMap.h"

class Reporter {
private:
//    static bool inited;
//    static jclass clsZ;
//    static void init();

    static void* p_report(void* args);

public:
    static void report(const char* name, HashMap& hashMap);

};


#endif //SUNRISE_REPORTER_H
