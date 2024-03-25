#ifndef AND_LIST_H
#define AND_LIST_H


#include "../android/AndroidClassWrapperBase.h"

class List : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    List(const jobject& core);

    List(const List&)= delete;
    List& operator=(const List&)=delete;

    jobject iterator();

protected:
    List(){}
};


#endif //AND_LIST_H
