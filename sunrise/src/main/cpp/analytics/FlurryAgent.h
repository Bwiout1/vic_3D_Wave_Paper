#ifndef SUNRISE_FLURRYAGENT_H
#define SUNRISE_FLURRYAGENT_H

#include "../android/AndroidClassWrapperBase.h"
#include "../util/HashMap.h"

class FlurryAgent : public AndroidClassWrapperBase  {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static void logEvent(jstring jstr);
    static void logEvent(jstring jstr, HashMap& map);

    static void setUserId(jstring jstr);

public:
    class Builder;
};


class FlurryAgent::Builder: public AndroidClassWrapperBase  {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Builder();

    Builder& withLogEnabled(jboolean flag);
    Builder& withLogLevel(jint i);
    Builder& withCaptureUncaughtExceptions(jboolean flag);

    void build(const jobject & context, const char* flurrykey);
};


#endif //SUNRISE_FLURRYAGENT_H
