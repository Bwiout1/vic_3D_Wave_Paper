#ifndef ADS_FAIRBID_MEDIATIONSTARTEDLISTENER_H
#define ADS_FAIRBID_MEDIATIONSTARTEDLISTENER_H


#include <functional>
#include "../../../reflect/BaseJavaInterface.h"

class MediationStartedListener : public BaseJavaInterface {
private:
    static bool inited;
    static jclass clsZ;
    static void init();

public:
    typedef void (* StartCallback)(jobject);
    typedef void (* FailedCallback)(jobject, jstring);

    MediationStartedListener(StartCallback start_callback, FailedCallback failed_callback);

    jobject invoke(jobject method, jobjectArray objects);


private:
    MediationStartedListener(const MediationStartedListener &) = delete;
    MediationStartedListener& operator=(const MediationStartedListener &)=delete;

    StartCallback mStartCallback;
    FailedCallback mFailedCallback;
};


#endif //ADS_FAIRBID_MEDIATIONSTARTEDLISTENER_H
