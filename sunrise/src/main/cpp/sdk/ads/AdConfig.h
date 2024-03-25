#ifndef SUNRISE_ADCONFIG_H
#define SUNRISE_ADCONFIG_H

#include <jni.h>


class AdConfig {
public:
    class MopubConfig;
    class PangleConfig;
    class IronSourceConfig;

};

class AdConfig::MopubConfig{
private:
    jstring nativeId;
    jstring intersId;

public:
    jstring getNativeId(){ return nativeId; };
    void setNativeId(jstring id){ nativeId = id; }
    jstring getIntersId(){ return intersId; };
    void setIntersId(jstring id){ intersId = id; }
};

class AdConfig::PangleConfig{
private:
    jstring appId;
    jstring nativeId;
    jstring intersId;

public:
    jstring getAppId(){ return appId; }
    void setAppId(jstring id){ appId = id; }
    jstring getNativeId(){ return nativeId; };
    void setNativeId(jstring id){ nativeId = id; }
    jstring getIntersId(){ return intersId; };
    void setIntersId(jstring id){ intersId = id; }
};

class AdConfig::IronSourceConfig{
private:
    jstring key;

public:
    jstring getKey(){ return key; }
    void setKey(jstring k){ key = k; }
};


#endif //SUNRISE_ADCONFIG_H
