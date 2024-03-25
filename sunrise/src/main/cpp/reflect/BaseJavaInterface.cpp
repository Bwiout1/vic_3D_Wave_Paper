#include "BaseJavaInterface.h"
#include "JavaClass.h"
#include "Proxy.h"
#include "NativeInvocationHandler.h"

const long BaseJavaInterface::MAGIC =   ((__TIME__)[0] - '0') * 100000 + ((__TIME__)[1] - '0') * 10000
                                           + ((__TIME__)[3] - '0') * 1000   + ((__TIME__)[4] - '0') * 100
                                           + ((__TIME__)[6] - '0') * 10     + ((__TIME__)[7] - '0')
                                           + __LINE__;


BaseJavaInterface::BaseJavaInterface() {
    mHandler = new NativeInvocationHandler(this);
}

long BaseJavaInterface::getMagicNum() {
    return magicNum;
}

jobject BaseJavaInterface::generateProxy(jclass interface) {
    return Proxy::newProxyInstance(JavaClass::getClassLoader(interface),
                                   JavaClass::getInterfaces(interface),
                                   mHandler->getCore());
}

BaseJavaInterface::~BaseJavaInterface() {
    if (mHandler== nullptr){
        delete mHandler;
        mHandler = nullptr;
    }

    magicNum = -1;
}


