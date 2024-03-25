#ifndef AND_COMPONENTNAME_H
#define AND_COMPONENTNAME_H

#include "AndroidClassWrapperBase.h"
#include "Context.h"


class ComponentName : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();
public:
    ComponentName(const jobject& componentname);

    ComponentName(jstring pkg, jstring cls);
    ComponentName(Context& context, jstring cls);
    ComponentName(const ComponentName&) = delete;
    ComponentName& operator=(const ComponentName&)=delete;

    jstring getPackageName();
};


#endif //AND_COMPONENTNAME_H
