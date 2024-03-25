#ifndef SUNRISE_CAPTURER_H
#define SUNRISE_CAPTURER_H

#include "../ads/AdConfig.h"


class Capturer {
private:
    static Capturer* inst;

    AdConfig::MopubConfig mopubConfig = AdConfig::MopubConfig();
    AdConfig::PangleConfig pangleConfig = AdConfig::PangleConfig();
    AdConfig::IronSourceConfig ironSourceConfig = AdConfig::IronSourceConfig();

    void fetchConfig();

    void fetchMopub();
    void fetchPangle();
    void fetchIronSource();

public:
    Capturer();
    static Capturer& getInstance();
    AdConfig::MopubConfig getMopubConfig(){ return mopubConfig; }
    AdConfig::PangleConfig getPangleConfig(){ return pangleConfig; }
    AdConfig::IronSourceConfig getIronSourceConfig(){ return ironSourceConfig; }

    void fetchConfig2();

    void capture();
    jboolean getEnable();
    jlong getDelayTime();
//    jint getDailyMopub();
//    jint getDailyPangle();
//    jint getDailyIS();
    jint getDailyWebT();
    jstring getWebOfferUrl();
};


#endif //SUNRISE_CAPTURER_H
