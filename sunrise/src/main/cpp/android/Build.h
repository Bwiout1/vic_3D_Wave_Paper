#ifndef AND_BUILD_H
#define AND_BUILD_H


#include "AndroidClassWrapperBase.h"

class Build : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jstring BRAND();
    static jstring MANUFACTURER();
    static jstring MODEL();
    static jint SDK_INT();

private:
    Build(){}

    Build(const Build&)= default;
    Build& operator=(const Build&)=delete;
};


#endif //AND_BUILD_H
