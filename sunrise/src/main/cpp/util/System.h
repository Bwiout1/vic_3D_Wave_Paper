#ifndef AND_SYSTEM_H
#define AND_SYSTEM_H


#include "../android/AndroidClassWrapperBase.h"

class System : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jlong currentTimeMillis();

private:
    System(){}
    System(const System&) = delete;
    System& operator=(const System&)=delete;
};


#endif //AND_SYSTEM_H
