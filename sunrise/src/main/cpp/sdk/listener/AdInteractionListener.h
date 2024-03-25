#ifndef SUNRISE_ADINTERACTIONLISTENER_H
#define SUNRISE_ADINTERACTIONLISTENER_H

#include "../../reflect/BaseJavaInterface.h"


class AdInteractionListener: public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

    const char* kind = "";

public:
    AdInteractionListener(const char* _kind);
    jobject invoke(jobject method, jobjectArray objects);

private:
    AdInteractionListener(const AdInteractionListener&)=delete;
    AdInteractionListener& operator=(const AdInteractionListener&)=delete;

};


#endif //SUNRISE_ADINTERACTIONLISTENER_H
