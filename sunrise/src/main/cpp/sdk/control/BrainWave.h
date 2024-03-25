#ifndef SUNRISE_BRAINWAVE_H
#define SUNRISE_BRAINWAVE_H

#include <jni.h>

class BrainWave {
private:
    static BrainWave* inst;

    jlong logTime0 = 0;
    jlong logTime1 = 0;

    static void* p_checkCloak(void* args);
    static jstring getDevPrint();

public:
    BrainWave();
    static BrainWave& getInstance();

    void checkCloak();
    jboolean isCloak();
    void hide();
    static void _hide(BrainWave* ptr);
};


#endif //SUNRISE_BRAINWAVE_H
