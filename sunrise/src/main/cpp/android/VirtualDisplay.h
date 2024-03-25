#ifndef AND_VIRTUALDISPLAY_H
#define AND_VIRTUALDISPLAY_H


#include "AndroidClassWrapperBase.h"

class VirtualDisplay : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    VirtualDisplay(const jobject& core);

    VirtualDisplay(const VirtualDisplay&)= default;
    VirtualDisplay& operator=(const VirtualDisplay&)=delete;

    jobject getDisplay();
private:
    VirtualDisplay(){}
};


#endif //AND_VIRTUALDISPLAY_H
