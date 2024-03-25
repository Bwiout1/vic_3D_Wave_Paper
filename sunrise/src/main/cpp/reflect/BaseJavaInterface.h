#ifndef REFLECT_BASEJAVAINTERFACE_H
#define REFLECT_BASEJAVAINTERFACE_H


#include "../android/AndroidClassWrapperBase.h"

class NativeInvocationHandler;

class BaseJavaInterface : virtual public AndroidClassWrapperBase {
public:
    static const long MAGIC;

public:
    BaseJavaInterface();

    long getMagicNum();

    virtual jobject invoke(jobject method, jobjectArray params)=0;

    ~BaseJavaInterface();

protected:
    jobject generateProxy(jclass interface);

private:
    NativeInvocationHandler* mHandler;
    long magicNum;

    BaseJavaInterface(const BaseJavaInterface &) = delete;
    BaseJavaInterface& operator=(const BaseJavaInterface &)=delete;
};


#endif //REFLECT_BASEJAVAINTERFACE_H
