#ifndef AND_ITERATOR_H
#define AND_ITERATOR_H


#include "../android/AndroidClassWrapperBase.h"

class Iterator : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Iterator(const jobject& core);

    Iterator(const Iterator&)= delete;
    Iterator& operator=(const Iterator&)=delete;

    jboolean hasNext();
    jobject next();

private:
    Iterator(){}
};


#endif //AND_ITERATOR_H
