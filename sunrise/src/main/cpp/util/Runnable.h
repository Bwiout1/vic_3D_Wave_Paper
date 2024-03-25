#ifndef AND_RUNNABLE_H
#define AND_RUNNABLE_H

#include <jni.h>
#include "../android/AndroidClassWrapperBase.h"
#include "../reflect/BaseJavaInterface.h"

typedef void (* RunnableCallback)(void*);

class Runnable :virtual public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Runnable(RunnableCallback callback, void *arg);

    Runnable(const Runnable&)=delete;
    Runnable& operator=(const Runnable&)=delete;

    jobject invoke(jobject method, jobjectArray objects);

private:
    Runnable(){};

    RunnableCallback mCallback;
    void* mArg;
};


#endif //AND_RUNNABLE_H
