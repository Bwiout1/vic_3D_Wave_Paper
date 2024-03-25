#ifndef ADS_FAIRBID_USERINFO_H
#define ADS_FAIRBID_USERINFO_H


#include "../../../android/AndroidClassWrapperBase.h"

class UserInfo : public AndroidClassWrapperBase {
private:
    static bool inited;
    static jclass clsZ;

    static void init();

public:
    static void setGdprConsent(jboolean haveConsent, const jobject& context);
    static void setLgpdConsent(jboolean haveConsent, const jobject& context);
};


#endif //ADS_FAIRBID_USERINFO_H
