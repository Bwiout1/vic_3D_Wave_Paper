#ifndef SUNRISE_INTERSSHOWCALLBACK_H
#define SUNRISE_INTERSSHOWCALLBACK_H

#include "IntersShowListener.h"


class IntersShowCallback: public IntersShowListener {
private:
    jobject next = nullptr;
public:
    IntersShowCallback(const jobject& _next);

    void onShow(jstring pre);
    void onShowFail(jstring pre);
    void onDismiss(jstring pre);
};


#endif //SUNRISE_INTERSSHOWCALLBACK_H
