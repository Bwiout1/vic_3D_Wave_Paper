#ifndef ADS_FAIRBID_H
#define ADS_FAIRBID_H


#include "../../../android/AndroidClassWrapperBase.h"

class FairBid : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static jobject configureForAppId(const char* app_id);
    static jboolean hasStarted();

public:
    FairBid(const jobject core);

    FairBid& enableLogs();
    FairBid& setUserAChild(jboolean child);
//    FairBid& disableAutoRequesting();
//    FairBid& disableAdvertisingId();
    FairBid& withMediationStartedListener(jobject listener);

    void start(jobject activity);
};


#endif //ADS_FAIRBID_H
