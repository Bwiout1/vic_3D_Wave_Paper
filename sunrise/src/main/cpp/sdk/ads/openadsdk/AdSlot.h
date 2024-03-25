#ifndef SUNRISE_ADSLOT_H
#define SUNRISE_ADSLOT_H


#include "../../../android/AndroidClassWrapperBase.h"


class AdSlot{
private:
    class Builder;
    static Builder* builder_native;
    static Builder* builder_inter;
    int kind = 0;
public:
    AdSlot(int num);
    Builder& newBuilder();
    jobject getCore();

    bool checkSafe();
};

class AdSlot::Builder : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();
public:
    Builder(Builder** p);
    bool isSafe = true;
    jobject adSlot = nullptr;

    Builder& setCodeId(jstring codeId);
    Builder& setSupportDeepLink(jboolean flag);
    Builder& setImageAcceptedSize(jint a, jint b);
    Builder& build();

    ~Builder();
};

#endif //SUNRISE_ADSLOT_H
