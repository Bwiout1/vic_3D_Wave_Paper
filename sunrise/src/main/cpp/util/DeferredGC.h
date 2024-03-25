#ifndef UTIL_DEFERREDGC_H
#define UTIL_DEFERREDGC_H


/**
 * 在某些情况下，不太方便/安全及时delete对象时，比如：
 * 1。在class的method中delete this
 * 2。Runnable运行后，需要删除Runnable对象
 * 。。。。。。
 * 可将该对象放入此处，此处统一调用delete
 */
namespace DeferredGC {
    class VirtualDestructor {
    public:
        virtual ~VirtualDestructor(){};
    };

    void recycle(VirtualDestructor* p);
}


#endif //UTIL_DEFERREDGC_H
