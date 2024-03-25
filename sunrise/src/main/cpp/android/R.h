#ifndef PAPERSDK_R_H
#define PAPERSDK_R_H


#include <jni.h>

class R {
public:
    static jint getLayoutId(const char* name);
    static jint getDrawableId(const char* name);
    static jint getMipmapId(const char* name);
};


#endif //PAPERSDK_R_H
