#ifndef SUNRISE_CONCURRENTLINKEDQUEUE_H
#define SUNRISE_CONCURRENTLINKEDQUEUE_H


#include "../android/AndroidClassWrapperBase.h"


class ConcurrentLinkedQueue: public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    ConcurrentLinkedQueue();
    void add(const jobject& obj);
    void addAll(const jobject& obj);
    jobject poll();
    jboolean isEmpty();

};


#endif //SUNRISE_CONCURRENTLINKEDQUEUE_H
