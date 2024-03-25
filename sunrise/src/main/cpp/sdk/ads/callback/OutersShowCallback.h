#ifndef SUNRISE_OUTERSSHOWCALLBACK_H
#define SUNRISE_OUTERSSHOWCALLBACK_H

#include "IntersShowListener.h"
#include <functional>


class OutersShowCallback : public IntersShowListener {
private:
    std::function<void()> next;
public:
    OutersShowCallback(std::function<void()> _next);

    void onShow(jstring pre);
    void onShowFail(jstring pre);
    void onDismiss(jstring pre);
};


#endif //SUNRISE_OUTERSSHOWCALLBACK_H
