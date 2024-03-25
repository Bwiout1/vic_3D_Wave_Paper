#ifndef SUNRISE_TTADSDK_INITCALLBACK_H
#define SUNRISE_TTADSDK_INITCALLBACK_H


#include "../../../reflect/BaseJavaInterface.h"


class TTAdSdk_InitCallback : public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    TTAdSdk_InitCallback();
    jobject invoke(jobject method, jobjectArray objects);

private:
    TTAdSdk_InitCallback(const TTAdSdk_InitCallback&)=delete;
    TTAdSdk_InitCallback& operator=(const TTAdSdk_InitCallback&)=delete;
};


#endif //SUNRISE_TTADSDK_INITCALLBACK_H
