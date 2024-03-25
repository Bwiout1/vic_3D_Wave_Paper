#ifndef AND_PRESENTATION_H
#define AND_PRESENTATION_H


#include "AndroidClassWrapperBase.h"

class Presentation : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Presentation(const jobject& core);

    Presentation(const jobject& outerContext, const jobject& display);
    Presentation(const Presentation&)= default;
    Presentation& operator=(const Presentation&)=delete;

    void show();

private:
    Presentation(){}
};


#endif //AND_PRESENTATION_H
