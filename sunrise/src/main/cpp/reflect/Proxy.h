#ifndef REFLECT_PROXY_H
#define REFLECT_PROXY_H

#include <jni.h>

class Proxy {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject newProxyInstance(jobject loader, jobject interfaces, jobject h);
};


#endif //REFLECT_PROXY_H
