#ifndef AND_REMOTEVIEWS_H
#define AND_REMOTEVIEWS_H


#include "AndroidClassWrapperBase.h"

class RemoteViews : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    RemoteViews(jstring packageName, jint layoutId);

    RemoteViews(const RemoteViews&)=delete;
    RemoteViews& operator=(const RemoteViews&)=delete;
};


#endif //AND_REMOTEVIEWS_H
