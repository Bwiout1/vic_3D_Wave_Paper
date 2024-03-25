#ifndef AND_CONFIGURATION_H
#define AND_CONFIGURATION_H


#include "AndroidClassWrapperBase.h"

class Configuration : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Configuration(const jobject& core);

    Configuration(const Configuration&)= default;
    Configuration& operator=(const Configuration&)=delete;

    jint densityDpi=0;
private:
    Configuration(){}
};


#endif //AND_CONFIGURATION_H
