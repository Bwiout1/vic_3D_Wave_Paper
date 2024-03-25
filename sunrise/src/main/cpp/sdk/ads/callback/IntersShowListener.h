#ifndef SUNRISE_INTERSSHOWLISTENER_H
#define SUNRISE_INTERSSHOWLISTENER_H

#include <jni.h>

class IntersShowListener {
public:
    virtual void onShow(jstring pre)=0;
    virtual void onShowFail(jstring pre)=0;
    virtual void onDismiss(jstring pre)=0;
};


#endif //SUNRISE_INTERSSHOWLISTENER_H
