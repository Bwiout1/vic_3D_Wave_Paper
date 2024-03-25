#ifndef AND_WORKREQUEST_H
#define AND_WORKREQUEST_H


#include "../AndroidClassWrapperBase.h"

class WorkRequest{
public:
    class Builder;
};

class WorkRequest::Builder : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Builder& setInitialDelay(jlong duration, const jobject timeUnit);
    jobject build();

private:
    Builder(const Builder&)= delete;
    Builder& operator=(const Builder&)=delete;

protected:
    Builder(){
        if(!inited){
            init();
        }
    }
};


#endif //AND_WORKREQUEST_H
