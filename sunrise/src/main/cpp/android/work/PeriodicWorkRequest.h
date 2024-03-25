#ifndef AND_PERIODICWORKREQUEST_H
#define AND_PERIODICWORKREQUEST_H


#include "WorkRequest.h"

class PeriodicWorkRequest : public WorkRequest {
public:
    class Builder;
};

class PeriodicWorkRequest::Builder : public WorkRequest::Builder {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:

    Builder(jclass workerClass, jlong repeatInterval, jobject repeatIntervalTimeUnit, jlong flexInterval, jobject flexIntervalTimeUnit);

    Builder(const Builder&)= delete;
    Builder& operator=(const Builder&)=delete;
};


#endif //AND_PERIODICWORKREQUEST_H
