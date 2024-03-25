#ifndef SUNRISE_CONTROLROOM_H
#define SUNRISE_CONTROLROOM_H

#include <jni.h>
#include "Capturer.h"
#include "Faceless.h"
#include "BrainWave.h"
#include "../../android/Application.h"
#include "../../android/AndroidClassWrapperBase.h"
#include "AppLifecycleManager.h"


class ControlRoom {
public:
    class Builder;

private:
    static bool inited;
    static ControlRoom* instance;

    Builder* builder = nullptr;

public:
    static ControlRoom& getInstance();
    Builder& getBuilder();

    Capturer& getCapturer();
    Faceless& getFaceless();
    BrainWave& getBrainWave();
    jlong getInitTime();
    jboolean getSdkEnable();
};


class ControlRoom::Builder: public AndroidClassWrapperBase{

public:
    Builder(Builder** p);

    Capturer* capturer = nullptr;
    Faceless* faceless = nullptr;
    BrainWave* brainWave = nullptr;
    //todo:何时释放

    jlong time;

    Builder& adLog();
    Builder& adConfig();
    Builder& cloak();
    Builder& run();

    void build();
};


#endif //SUNRISE_CONTROLROOM_H
