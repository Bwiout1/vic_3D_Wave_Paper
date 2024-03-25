#ifndef AND_RESOURCES_H
#define AND_RESOURCES_H


#include "AndroidClassWrapperBase.h"
#include "../util/InputStream.h"

class Resources  : public AndroidClassWrapperBase{
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Resources(const jobject& core);

    Resources(const Resources&)= default;
    Resources& operator=(const Resources&)=delete;

    jobject openRawResource(jint resId);
    jint getIdentifier(jstring name, jstring type, jstring pkg);
    jobject getConfiguration();
private:
    Resources(){}
};


#endif //AND_RESOURCES_H
