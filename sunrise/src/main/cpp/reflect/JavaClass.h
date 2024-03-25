#ifndef REFLECT_JAVACLASS_H
#define REFLECT_JAVACLASS_H

#include <jni.h>


class JavaClass {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject getClassLoader(jclass cls);

    static jobject getInterfaces(jclass cls);

    static jstring getName(jclass cls);

    static jstring getSimpleName(const jobject& obj);

};


#endif //REFLECT_JAVACLASS_H
