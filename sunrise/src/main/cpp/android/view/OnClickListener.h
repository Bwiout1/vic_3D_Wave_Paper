#ifndef AND_ONCLICKLISTENER_H
#define AND_ONCLICKLISTENER_H

#include "../../reflect/BaseJavaInterface.h"
#include <functional>

class OnClickListener : public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

    std::function<void()> onClick;

public:
    OnClickListener();
    void overrideOnClick(std::function<void()> _onClick);
    jobject invoke(jobject method, jobjectArray objects);

private:
    OnClickListener(const OnClickListener&)=delete;
    OnClickListener& operator=(const OnClickListener&)=delete;
};


#endif //AND_ONCLICKLISTENER_H
