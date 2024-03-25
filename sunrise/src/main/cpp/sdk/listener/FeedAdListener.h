#ifndef SUNRISE_FEEDADLISTENER_H
#define SUNRISE_FEEDADLISTENER_H

#include "../../reflect/BaseJavaInterface.h"


class FeedAdListener : public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    FeedAdListener();
    jobject invoke(jobject method, jobjectArray objects);

private:
    FeedAdListener(const FeedAdListener&)=delete;
    FeedAdListener& operator=(const FeedAdListener&)=delete;

};


#endif //SUNRISE_FEEDADLISTENER_H
