#ifndef AND_RESOLVEINFO_H
#define AND_RESOLVEINFO_H


#include "AndroidClassWrapperBase.h"

class ResolveInfo : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject activityInfo(const jobject & core);

};


#endif //AND_RESOLVEINFO_H
