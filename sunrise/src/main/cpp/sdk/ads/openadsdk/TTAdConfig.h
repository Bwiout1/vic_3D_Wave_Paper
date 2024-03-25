#ifndef SUNRISE_TTADCONFIG_H
#define SUNRISE_TTADCONFIG_H

#include "../../../android/AndroidClassWrapperBase.h"


class TTAdConfig{
private:
    class Builder;
    static Builder* builder;

public:

    static Builder& newBuilder();
    static jobject getCore();

};

class TTAdConfig::Builder: public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    jobject ttAdConfig = nullptr;
    Builder(Builder** p);

    Builder& appId(jstring adId);
    Builder& appName(jstring name);
    Builder& useTextureView(jboolean flag);
    Builder& allowShowPageWhenScreenLock(jboolean flag);
    Builder& debug(jboolean flag);
    Builder& supportMultiProcess(jboolean flag);
    Builder& coppa(jint num);
    Builder& setGDPR(jint num);
    Builder& setCCPA(jint num);

    void build();

    ~Builder();
};


#endif //SUNRISE_TTADCONFIG_H
