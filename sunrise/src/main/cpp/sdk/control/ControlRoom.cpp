#include "ControlRoom.h"
#include "../../util/Logger.h"
#include "../support/Switch.h"
#include "../../util/System.h"

bool ControlRoom::inited = false;
ControlRoom* ControlRoom::instance = nullptr;

ControlRoom &ControlRoom::getInstance() {
    if(!inited){
        instance = new ControlRoom();
        inited = true;
    }

    return *instance;
}

Capturer& ControlRoom::getCapturer() {
    return *(builder->capturer);
}

Faceless& ControlRoom::getFaceless() {
    return *(builder->faceless);
}

BrainWave& ControlRoom::getBrainWave() {
    return *(builder->brainWave);
}

jlong ControlRoom::getInitTime() {
    return builder->time;
}

jboolean ControlRoom::getSdkEnable() {
    if(builder == nullptr){
        logd("builder is null");
        return false;
    }

    if(!getBrainWave().isCloak()){
        logd("!getBrainWave().isCloak() is true");
    }
    if(getCapturer().getEnable()){
        logd("getCapturer().getEnable() is true");
    }

    return !getBrainWave().isCloak() && getCapturer().getEnable();
}

ControlRoom::Builder &ControlRoom::getBuilder() {
    new Builder(&builder);
    return *builder;
}


/*----------------------------------------Builder-------------------------------------------------*/


ControlRoom::Builder::Builder(Builder** p) {
    *p = this;
}

ControlRoom::Builder &ControlRoom::Builder::adLog() {
    logd("mopub enable :%s", Switch::mopub_enable? "true":"false");
    logd("pangle enable :%s", Switch::pangle_enagle? "true":"false");
    logd("ironSource enable :%s", Switch::ironsource_enable? "true":"false");
    logd("offer enable :%s", Switch::offer_enable? "true":"false");

    return *this;
}

ControlRoom::Builder &ControlRoom::Builder::adConfig() {
    *capturer = Capturer::getInstance();
    return *this;
}

ControlRoom::Builder &ControlRoom::Builder::cloak() {
    *brainWave = BrainWave::getInstance();
    return *this;
}

ControlRoom::Builder &ControlRoom::Builder::run() {
    *faceless = Faceless::getInstance();
    return *this;
}

void ControlRoom::Builder::build() {

    AppLifecycleManager::getInstance().registerCallback();
    capturer->capture();
    brainWave->checkCloak();
    faceless->run();

    time = System::currentTimeMillis();
    logd("init: %ld", time);
}
