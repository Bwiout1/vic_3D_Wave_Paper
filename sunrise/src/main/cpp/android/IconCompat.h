#ifndef AND_ICONCOMPAT_H
#define AND_ICONCOMPAT_H


#include "AndroidClassWrapperBase.h"


class IconCompat : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject createWithResource(const jobject& context, jint resId);

public:
    IconCompat(const jobject& core);

    IconCompat(const IconCompat&)= default;
    IconCompat& operator=(const IconCompat&)=delete;

private:
    IconCompat(){};
};


#endif //AND_ICONCOMPAT_H
