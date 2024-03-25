#ifndef SUNRISE_OUTERMANAGER_H
#define SUNRISE_OUTERMANAGER_H

#include <jni.h>

class OuterManager {
private:
    static OuterManager* inst;
    static void* p_run(void* args);

    jlong getInstallTime();
    jlong getDelayTime();
    jboolean isScreenOn();
    void start(jint type);

public:
    static OuterManager& getInstance();
    void run();
};


#endif //SUNRISE_OUTERMANAGER_H
