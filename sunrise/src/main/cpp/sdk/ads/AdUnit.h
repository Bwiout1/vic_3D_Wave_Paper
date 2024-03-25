#ifndef SUNRISE_ADUNIT_H
#define SUNRISE_ADUNIT_H

#include <jni.h>
#include "../../android/view/ViewGroup.h"
#include "callback/IntersShowListener.h"


class AdUnit {
public:
    virtual jstring name()=0;

    virtual void loadNative()=0;
    virtual void fillNative(ViewGroup& container)=0;
    virtual jboolean hasNative()=0;
    virtual void clearNative(ViewGroup& container)=0;

    virtual void loadInters()=0;
    virtual void showInters(const jobject& activity, IntersShowListener& callback)=0;
    virtual void loadNextInters()=0;
    virtual jboolean hasInters()=0;
};


#endif //SUNRISE_ADUNIT_H
