#ifndef AND_ONETIMEWORKREQUEST_H
#define AND_ONETIMEWORKREQUEST_H

#include "WorkRequest.h"

class OneTimeWorkRequest : public WorkRequest {
public:
    class Builder;
};

class OneTimeWorkRequest::Builder : public WorkRequest::Builder {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    Builder(jclass workerClass);

    Builder(const Builder&)= delete;
    Builder& operator=(const Builder&)=delete;
};


#endif //AND_ONETIMEWORKREQUEST_H
